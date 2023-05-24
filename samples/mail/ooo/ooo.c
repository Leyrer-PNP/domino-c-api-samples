/*************************************************************************
 *
 * Copyright HCL Technologies 1996, 2023.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *

    PROGRAM:   ooo

    FILE:      ooo.c

    PURPOSE:   This program will sets and gets user Out Of Office information.

    SYNTAX:    ooo


*************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */
#include <stdio.h>
#include<time.h>

/* Notes API include files */
#if defined (OS390)
#include <lapicinc.h>
#endif


#if defined (LINUX)
#include <string.h>
#endif


#include <global.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <ostime.h>
#include <osmisc.h>
#include <osenv.h>

#include <osfile.h>
#include <mail.h>
#include <mailserv.h> 

#include <oooapi.h>

/* Local function prototypes */
void PrintAPIError (STATUS);
STATUS getCurrDate(char* stDate, char *enDate);

#define MAIL_MAILSERVER_ITEM "MailServer"
#define SECOND               1
#define MINUTE               60 * SECOND
#define HOUR                 60 * MINUTE
#define DAY                  24 * HOUR

#define MESSAGELEN           500

/* Notes API subroutine */
int main (int argc, char *argv[])
{
        WORD            TimeStringLen;
        STATUS          error = NOERROR;   /* return code from API calls */
        OOOCTXHANDLE    hOOOContext = NULLHANDLE;
        OOOCTXPTR       *pOOOContext = NULL;
        TIMEDATE        tdStartAway,tdEndAway;
        char            timetext[MAXALPHATIMEDATE+1];   /* time/date in ASCII */
        char            far *text_pointer;              /* pointer to timetext */
        DHANDLE         hMailFile = NULLHANDLE;

        char            *szMailFileName;
        char            *szOwnerName;
        char            szServerName[MAXUSERNAME+1];
        char            szMailFilePath[MAXPATH+1];
        char            szStartAway[MAXALPHATIMEDATE+1],szEndAway[MAXALPHATIMEDATE+1];
        char            pGeneralMessage[MESSAGELEN];
        char            pGeneralSubject[MESSAGELEN];
        WORD            retVersion,retState,pGeneralMessageLen;
        BOOL            bExcludeInternet;

        if (argc != 3)
        {
          printf( "\nUsage:  %s <user's mail database filename> <user name> \n", argv[0] );
          return (0);
        }


        /* init ... */
        szOwnerName=argv[2];
        szMailFileName=argv[1];
        
        memset(szStartAway, '\0', sizeof(szStartAway));
        memset(szEndAway, '\0', sizeof(szEndAway));
        getCurrDate(szStartAway, szEndAway);
        printf("Start date[ %s ]- End date[ %s ]\n", szStartAway, szEndAway);
        fflush(stdout);

        strcpy(pGeneralMessage,szOwnerName);
        strcat(pGeneralMessage," will out of office");
        strcpy(pGeneralSubject,szOwnerName);
        strcat(pGeneralSubject," is out of office");

        if ( error = NotesInitExtended (argc, argv) )
        {
            printf("\n Unable to initialize Notes. Error Code[0x%04x]\n", error);
            fflush(stdout);
            return (1);
        }

        if (!OSGetEnvironmentString(MAIL_MAILSERVER_ITEM, szServerName, MAXUSERNAME))
        {
           printf ("\nUnable to get mail server name ...\n\n ");
           fflush(stdout);
           strcpy(szServerName,"");
           NotesTerm();
           return(1);
        }


        OSPathNetConstruct( NULL, szServerName, szMailFileName,szMailFilePath);

        if (error = MailOpenMessageFile(szMailFilePath, &hMailFile))
        {
            PrintAPIError (error);
            NotesTerm();
            return (1);
        }

/* at add-in initialization */
        if (error = OOOInit())
        {
            PrintAPIError (error);
            goto EXIT2;
        }

/* Enable OOO for one specific user */

        if ( error = OOOStartOperation(szOwnerName,
                                       szServerName,
                                       TRUE,
                                       hMailFile,
                                       &hOOOContext,
                                       &pOOOContext))
        {
            PrintAPIError (error);
            goto EXIT1;

        }

        if ( error = OOOGetState(pOOOContext,&retVersion,&retState))
        {
            PrintAPIError (error);
            goto EXIT1;
        }

        switch( retVersion )
        {
                case OOO_CONFIG_AGENT:
                        printf("The version (agent, service) of the out of office functionality is agent\n");
                        break;
                case OOO_CONFIG_SERVICE:
                        printf("The version (agent, service) of the out of office functionality is service\n");
                        break;
        }

        switch ( retState )
        {
                case OOO_STATE_DISABLED:
                        printf("The state (disabled, enabled) of the out of office functionality is disabled\n");
                        break;
                case OOO_STATE_ENABLED:
                        printf("The state (disabled, enabled) of the out of office functionality is enabled\n");
                        break;

        }

        if (OOOSetExcludeInternet(pOOOContext,TRUE))
        {
           PrintAPIError (error);
           goto EXIT1;

        }


        if (OOOGetExcludeInternet(pOOOContext,&bExcludeInternet))
        {
           PrintAPIError (error);
           goto EXIT1;

        }

        if (bExcludeInternet)
        {
           printf("Gets ExcludeInternet is TRUE\n");
        }
        else 
        {
          printf("Gets ExcludeInternet is FALSE\n");
        }

        strcpy (timetext, szStartAway);

/* Get a pointer to the time/date string. We need this for the next call. */

        text_pointer = szStartAway;

/* Put the time/date text into Domino and Notes binary form. */

        if (error = ConvertTextToTIMEDATE(NULL,NULL,
                                          &text_pointer,(WORD) strlen(szStartAway),&tdStartAway))
        {
            PrintAPIError (error);
            goto EXIT1;

        }

        strcpy (timetext, szEndAway);
/* Get a pointer to the time/date string. We need this for the next call. */

        text_pointer = timetext;

/* Put the time/date text into Domino and Notes binary form. */

        if (error = ConvertTextToTIMEDATE(NULL,
                      NULL,
                      &text_pointer,
                      (WORD) strlen(timetext),
                      &tdEndAway))
        {
           PrintAPIError (error);
           goto EXIT1;

        }

        if (error = OOOSetAwayPeriod(pOOOContext, tdStartAway, tdEndAway))
        {
            PrintAPIError (error);
            goto EXIT1;

        }

        if (error = OOOEnable(pOOOContext, TRUE))
        {
            PrintAPIError (error);
            goto EXIT1;

        }

        if (OOOSetGeneralSubject(pOOOContext,pGeneralSubject,TRUE))
        {
            PrintAPIError (error);
            goto EXIT1;

        }

        if (OOOGetGeneralSubject(pOOOContext,pGeneralSubject))
        {
            PrintAPIError (error);
            goto EXIT1;

        }

        printf("Gets OOO General Subject:\n");
        printf("%s\n",pGeneralSubject);
        fflush(stdout);

        if (OOOSetGeneralMessage(pOOOContext,pGeneralMessage,sizeof(pGeneralMessage)))
        {
            PrintAPIError (error);
            goto EXIT1;
        }


        if (OOOGetGeneralMessage(pOOOContext,pGeneralMessage,&pGeneralMessageLen))
        {
            PrintAPIError (error);
            goto EXIT1;
        }

        printf("Gets OOO General Message:\n");
        printf("%s ",pGeneralMessage);
        fflush(stdout);

        if (error = OOOGetAwayPeriod(pOOOContext,&tdStartAway,&tdEndAway))
        {
            PrintAPIError (error);
            goto EXIT1;
        }

        if (error = ConvertTIMEDATEToText(NULL, NULL,&tdStartAway,
                            timetext, MAXALPHATIMEDATE,
                            &TimeStringLen))
        {
            PrintAPIError (error);
            goto EXIT1;
        }

        timetext[TimeStringLen] = '\0';
        printf("%s to ", timetext);


        if (error = ConvertTIMEDATEToText(NULL, NULL,&tdEndAway,
                            timetext, MAXALPHATIMEDATE,
                            &TimeStringLen))
        {
            PrintAPIError (error);
            goto EXIT1;
        }

        timetext[TimeStringLen] = '\0';
        printf("%s.", timetext);


        if (hOOOContext && pOOOContext)
        {
            if (error = OOOEndOperation(hOOOContext, pOOOContext))
            {
                PrintAPIError (error);
                goto EXIT1;
            }
        }

/* at add-in termination */
EXIT1:
        if (error = OOOTerm())
        {
            PrintAPIError (error);
            NotesTerm();
            return (1);
        }
EXIT2:
        if (hMailFile != NULL) 
        {
           if (error = MailCloseMessageFile(hMailFile))
           {
               PrintAPIError (error);
               NotesTerm();
               return (1);
           }
        }

    /* End of subroutine. */
        printf("\nProgram completed successfully.\n");
        fflush(stdout);
        NotesTerm();
        return (0);
}

/* This function is to get the start and end date for ooo */
/* To manipulate this set NotesDateFormat=1 for DMY 
 * NotesDateFormat=2 for YMD
 * for 3 MDY in notes.ini and restart notes client or server accordingly
 */

STATUS getCurrDate(char *stDate, char *enDate)
{
       time_t t = time(NULL);
       struct tm *time = localtime(&t);

#if defined (LINUX)
       sprintf(stDate, "%02d/%02d/%d", time->tm_mon + 1,  time->tm_mday , time->tm_year + 1900);
       t = t + (1 * DAY);
       time = localtime(&t);
       sprintf(enDate, "%02d/%02d/%d", time->tm_mon + 1,  time->tm_mday , time->tm_year + 1900);
#else
       sprintf(stDate, "%02d/%02d/%d",  time->tm_mday ,time->tm_mon + 1, time->tm_year + 1900);
       t = t + (1 * DAY);
       time = localtime(&t);
       sprintf(enDate, "%02d/%02d/%d", time->tm_mday, time->tm_mon + 1, time->tm_year + 1900);
#endif
       return NOERROR;
}

/* This function prints the HCL C API for Notes/Domino error message
   associated with an error code. */

void PrintAPIError (STATUS api_error)

{
    STATUS  string_id = ERR(api_error);
    char    error_text[200];
    WORD    text_len;

    /* Get the message for this HCL C API for Notes/Domino error code
       from the resource string table. */

    text_len = OSLoadString (NULLHANDLE,
                             string_id,
                             error_text,
                             sizeof(error_text));

    /* Print it. */
    fprintf (stderr, "\n%s\n", error_text);

}
