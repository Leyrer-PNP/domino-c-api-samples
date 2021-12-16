/**************************************************************************
                
    PROGRAM:    namebook

    FILE:       namebook.c

    PURPOSE:    Demonstrate the HCL C API for Notes/Domino 
                function NAMEGetAddressBooks().

    SYNTAX:     namebook  <server name>  [options]

        if options = TITLES, this prints the title of each database in
        addition to file path.

**************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif


/* OS and C include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* HCL C API for Notes/Domino include files */
#include <global.h>
#include <intl.h>
#include <nsf.h>
#include <acl.h>
#include <misc.h>
#include <osmem.h>
#include <osenv.h>
#include <osfile.h>                     /* OSPathNetParse */
#include <ostime.h>
#include <lookup.h>
#include <stdnames.h>
#include <textlist.h>
#include <editods.h>
#include <oserr.h>
#include <osmisc.h>

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif

void PrintAPIError (STATUS);
 
/************************************************************************

    FUNCTION:   Main

*************************************************************************/

int main (int argc, char *argv[])
{
    char    szServer[MAXUSERNAME+1];
    char*   szOptions;
    WORD    wOption;
    STATUS  error=NOERROR;
    WORD    wCount;
    WORD    wLength;
    WORD    wEntry;
    WORD    wEntryLen;
    DHANDLE  hReturn;
    char   *pszReturn;    
    char    achPort[MAXPATH];
    char    achServer[MAXPATH];
    char    achFile[MAXPATH];
    char    szNetPathName[MAXPATH];         /* Network Path of Database */
    char    szPortName[MAXPATH];         /* Port Name */

    if (argc<1 || argc>2)
    {
Syntax:
        printf ("Syntax: namebook  [option]\n");
        printf (
        "where [option] = TITLES or nothing.\n");
        return NOERROR;
    }

    wOption = 0;

    if (argc==2)
    {
        szOptions=argv[1];
        if (!strcmp(szOptions,"TITLES"))
            wOption = NAME_GET_AB_TITLES | NAME_DEFAULT_TITLES ;
        else
        {
            printf ("Error: unrecognized option '%s'\n", szOptions);
            goto Syntax;
        }
    }
        

    if (error=NotesInitExtended (argc, argv))
    {
       printf("\n Unable to initialize Notes. Error Code[0x%04x]\n", error);    
       return (1);
    }
    
    /* get the Mail Server from notes.ini */
    if (!OSGetEnvironmentString("MailServer", szServer, MAXUSERNAME))
    {
        NotesTerm();
        printf("\nError in getting mail server name. Exiting");
        return (1);
    }
    /* get the port name from notes.ini */
    if (!OSGetEnvironmentString("Ports", szPortName, MAXPATH))
    {
        NotesTerm();
        printf("\nError in getting port name. Exiting");
        return (1);
    }

    error = NAMEGetAddressBooks( 
                szServer,
                wOption,
               &wCount,
               &wLength,
               &hReturn);

    if (error)
       goto Bye;

    /* If there are no entries, return. */
    if (!wCount) 
    {
        printf ("No entries in NAMES.\n");
        goto Bye;
    }

    /* Lock the return buffer. */

    pszReturn = OSLock(char, hReturn);

    /* Loop through the entries. */

    for (wEntry = 0; wEntry < wCount; wEntry++)
    {
         /* Get path length. */
        wEntryLen = strlen(pszReturn);
        error = OSPathNetConstruct (szPortName, (char FAR *)szServer,
                                 pszReturn, (char FAR *)szNetPathName);
        if (error != NOERROR)
           goto Bye;

        /* Parse the path into components. */
        OSPathNetParse(szNetPathName, achPort, achServer, achFile);

        /* process components */
        printf("Entry %d\n\tPort = %s\n\tServer = %s\n\tFile = %s\n",
                (wEntry+1), achPort, achServer, achFile);

        if (wOption & NAME_GET_AB_TITLES)
        {
            /* Advance to title. */
            pszReturn += wEntryLen+1;
            wEntryLen = strlen(pszReturn);
            printf ("\tTitle = '%s'\n", pszReturn);
        }
          
        /* Advance pointer to next entry. */
        pszReturn += wEntryLen+1;
    }

    /* Unlock and free the return buffer. */
    OSUnlock(hReturn);
    OSMemFree(hReturn);

Bye:
    if (error)
        PrintAPIError (error);  
    NotesTerm();
    return(error);
}


/*************************************************************************

    FUNCTION:   PrintAPIError

    PURPOSE:    This function prints the HCL C API for Notes/Domino 
                error message associated with an error code.

**************************************************************************/

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

