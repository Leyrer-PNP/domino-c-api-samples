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

PROGRAM:    ADMINReqRenameExt

FILE:       ADMINReqRenameExt.c

PURPOSE:    Shows the usage of ADMINReqRenameExt API.

SYNTAX:     ADMINReqRenameExt  <server name> "names.nsf"

*************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files */

#include "global.h"
#include "nsfdb.h"
#include "nsfdata.h"
#include "osmisc.h"
#include "osfile.h"
#include "reg.h"
#include "adminp.h"


#if defined(CAPI_TESTING) 
#include "printlog.h" 
#else
 #define PRINTLOG printf 
 #define PRINTERROR(api_error, api_name) {\
 char    szErrorText[256] = { 0 };\
 OSLoadString(NULLHANDLE, ERR(api_error), szErrorText, sizeof(szErrorText));\
 fprintf(stderr, "[ERROR]:%s:%d:%s - %s", __FILE__,__LINE__,api_name,szErrorText); }
#endif 

/* constant types */
 #define     STDIN                   stdin

/* Local function prototypes */

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                               char *server_name, char *db_name);


STATUS GetCertCtx (char far *, HCERTIFIER *, char far *);

TIMEDATE ExpireDate;

/* Program declaration */
int main(int argc, char *argv[])
{
    /* Local data declarations */

    STATUS      error = NOERROR;             /* error code from API calls */
    DBHANDLE    hNABook = NULLHANDLE;        /* handle to NAB */
    NOTEID      NoteID = 0;                  /* noteID */
    NOTEHANDLE  nhNote = NULLHANDLE;         /* handle to note*/ 
    HCERTIFIER  hCertCtx = NULLHANDLE;       /* handle to certifier context */
    ADMINReqParams ARPptr;                   /* ADMINReqParam structure*/
    BOOL        logged, ferror;              /* output params*/
    char        USER_NAME[] = "CN=test user/O=HCLPNP";           /* user to be renamed*/
    char        ORGUNIT_CERT_ID[] = "C:\\Domino\\data\\cert.id"; /* Organization cert ID*/
    char        PASSWORD[] = "password";                         /* password for cert id*/
    char        NEW_FIRST_NAME[]= "A";                           /* new first name */
    char        NEW_LAST_NAME[]= "B";                            /* new last name */
    char        NEW_MIDDLE_NAME[]= "C";                          /* new middle name */
    char        USERS_FLD[] = "$Users";                          /* view to search for the user*/
    char        szServer[MAXUSERNAME]={0};                       /* server name */
    char        szDBName[MAXPATH]={0};                           /* database name*/
    char       *pszServerName = NULL;                            /* pointer to servername*/   
    char       *pszNABFile = NULL;                               /* pointer to NAB file*/
    char        szNABFilePath[MAXPATH];                          /* NAB file path*/
	
    pszServerName = szServer;
    pszNABFile = szDBName;
	
    /* Process input arguments */
    ProcessArgs(argc, argv, pszServerName, pszNABFile);
	
    /* Initialize the notes */
    if (error = NotesInitExtended(argc, argv))
    {
        PRINTERROR (error,"NotesInitExtended");
        return(1);
    }

    /* Construct the path for the admin request file */
    if (error = OSPathNetConstruct(NULL, pszServerName, pszNABFile, szNABFilePath))
    {
        PRINTERROR(error,"OSPathNetConstruct");
        NotesTerm();
        return (1);
    }
	
    /* Open the database. */
    if (error = NSFDbOpen(szNABFilePath, &hNABook))
    {	
	PRINTERROR (error,"NSFDbOpen");
	NotesTerm();
	return (1);
    }

    /* Get the Note ID of the user to be renamed. */
    if (error = REGFindAddressBookEntry (hNABook, USERS_FLD, USER_NAME, &NoteID))
    {
	NSFDbClose (hNABook);
	PRINTERROR (error,"REGFindAddressBookEntry");
	NotesTerm();
	return (1);
    }

    /* Get the Note handle. */
    if (error = NSFNoteOpen(hNABook, NoteID, 0, &nhNote))
    {
	NSFDbClose (hNABook);
	PRINTERROR (error,"NSFNoteOpen");
	NotesTerm();
	return (1);
    }

    /* Get the current certifier context */
    if (error = GetCertCtx(ORGUNIT_CERT_ID, &hCertCtx, PASSWORD))
    {
	NSFDbClose (hNABook);
	PRINTERROR (error,"GetCertCtx");
	NotesTerm();
	return (1);
    }
	
    /* Intializing the ADMINReqParams structure */ 
    memset(&ARPptr, 0x00, sizeof(ARPptr));
	
    /* Call the ADMINReqRenameExt API */
    if (error = ADMINReqRenameExt( hCertCtx,         /* certifier context */
	                           hNABook,          /* handle to NAB file */
	                           nhNote,           /* handle to notes document */
	                           NEW_FIRST_NAME,   /* new first name */
	                           NEW_MIDDLE_NAME,  /* new middle initial */
	                           NEW_LAST_NAME,    /* new last name */
	                           NULL,             /* new org name */
	                           NULL,             /* new short name */
	                           NULL,             /* new internet address */
	                           &logged,          /* TRUE if request has been recorded in the certification log */
	                           &ferror,          /* TRUE if an error occurred */
	                           &ARPptr,          /* pointer to an ADMINReqParams structure */
	                           sizeof(ARPptr)))  /* size of the buffer (ADMINReqParams structure) */
    {
	SECKFMFreeCertifierCtx (hCertCtx);
	NSFDbClose (hNABook);
	PRINTERROR (error,"ADMINReqRenameExt");
	NotesTerm();
	return (1);
    }

    PRINTLOG("!!! ADMINReqRenameExt Processed Sucessfully !!!\n ");
	
    /* Close the database. */
    if (error = NSFDbClose (hNABook))
    {
	PRINTERROR (error,"NSFDbClose");
	NotesTerm();
	return (1);
    }

}
/************************************************************************

    FUNCTION:    GetCertCtx

    PURPOSE:     returns a handle to the certifier context.

    INPUTS:
        pCertFilePath - pointer to full path spec of certifier id file
        szPassword - password

    OUTPUTS:
        phCertCtx     - pointer to handle to Certifier context

    RETURNS:    status code from API Call

*************************************************************************/

STATUS GetCertCtx (char far *pCertFile, HCERTIFIER *phCertCtx,
                   char far *szPassword)
{
    TIMEDATE ExpDate;
    STATUS error = NOERROR;
    char CertName[MAXSPRINTF] = {0};    /* Note:  Org names may be up to 64 chars
                                   The CertName in this example is an org
                                   name plus a  country code. */
    BOOL IsHierarchical = FALSE;
    WORD FileVersion = 0;
    KFM_PASSWORD KFMPassword, *pKFMPassword;    /* encoded password */

    OSCurrentTIMEDATE(&ExpDate);

    /* set the expiration date to two years from today (Domino and Notes default) */
    error = TimeDateAdjust(&ExpDate, 0, 0, 0, 0, 0, 2);
    ExpireDate = ExpDate;

    if (error)
        return (error);

   /* get the encoded password */
    if (szPassword == NULL)
        pKFMPassword = NULL;
    else
    {
        pKFMPassword = &KFMPassword;
        SECKFMCreatePassword (szPassword, pKFMPassword);
    }

   /* get the certifier context */
    error = SECKFMGetCertifierCtx (
                                   pCertFile,       /* Certifier file path spec */
                                   pKFMPassword,    /* encoded password */
                                   NULL,            /* no certifier log */
                                   &ExpDate,        /* expiration date */
                                   CertName,        /* returned name of certifier */
                                   phCertCtx,       /* returned handle to certifier context */
                                   &IsHierarchical, /* returned TRUE if certifier is
                                   hierarchical */
                                   &FileVersion);   /* returned file version */


    return (error);
}

/************************************************************************

    FUNCTION:   ProcessArgs

    INPUTS:     argc, argv - directly from the command line

    OUTPUTS:    server_name - server name obtained from command line or
                              from prompt.
                db_name -     database name obtained from command line or
                              from prompt.

*************************************************************************/

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                               char *server_name, char *db_name)
{
    if (argc != 3)
    {

        printf("Enter server name: ");
        fflush (stdout);
        fgets(server_name, MAXUSERNAME, STDIN);
        printf("\n");
        printf ("Enter database filename:  ");
        fflush (stdout);
        fgets(db_name, MAXPATH-1, STDIN);
    }
    else
    {
        memset(server_name, '\0', MAXUSERNAME);    
        strncpy(server_name, argv[1], MAXUSERNAME-1);
        memset(db_name, '\0', MAXPATH);    
        strncpy(db_name, argv[2], MAXPATH-1);
    } /* end if */
} /* ProcessArgs */

