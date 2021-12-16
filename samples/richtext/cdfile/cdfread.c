/****************************************************************************

    PROGRAM:    cdfread

    FILE:       cdfread.c

    SYNTAX:     cdfread

    PURPOSE:    Demonstrate function CompoundTextAssimilateFile.

    DESCRIPTION:
        This creates a new data note in the database, richtext.nsf 
        and fills in the rich text field of the note with the 
        compound text from a CD file, cd_recs.inp.  
        
        A CD file is a file containing Domino and Notes rich text data. 
		This data must be in Domino and Notes Canonical format. Use the 
		companion program "cdfwrite" to create this CD file.
        
        The database used in this program has a form named 
        "RichTextForm".  This form contains a time/date field named,
        TIME_DATE, and a rich text field named, RICH_TEXT.

****************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */

#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files */

#include <global.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <editods.h>
#include <editdflt.h>
#include <stdnames.h>
#include <ostime.h>
#include <easycd.h>
#include <fontid.h>
#include <osmem.h>
#include <nif.h>

#include <osmisc.h>

/* local include files */

#include "cdfile.h"

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif

void PrintAPIError (STATUS);

/************************************************************************

    FUNCTION:   main

*************************************************************************/

int main(int argc, char *argv[])
{
    STATUS      nErr = NOERROR; /* return code from API calls */
    char        szPathName[] = "richtext.nsf";     /* pathname of database */
    char        szItemName_RICH_TEXT[] = "RICH_TEXT";
    DBHANDLE    hDB;            /* database handle */
    NOTEHANDLE  hNote;          /* note handle */
    TIMEDATE    timedate;       /* a time/date field */
    DHANDLE       hCompound;      /* handle to CompoundText context */

    if (nErr = NotesInitExtended (argc, argv))
    {
        printf("\n Unable to initialize Notes.\n");
        return (1);
    }


    /* Open the database. */

    if ( (nErr = NSFDbOpen (szPathName, &hDB)) != NOERROR )
    {
        printf( "Error: unable to open database '%s'.\n", szPathName );
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }
   
   /* Create a new data note. */

    if ( (nErr = NSFNoteCreate (hDB, &hNote)) != NOERROR )
    {
        printf( "Error: unable to create new document in database.\n" );
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

   /* Write a field named FORM to the note -- this field specifies the
      default form to use when the note is displayed. */

    nErr = NSFItemSetText( hNote, FIELD_FORM, "RichTextForm", MAXWORD );

    if (nErr != NOERROR)
    {
        printf( "Error: unable to set text in item '%s'.\n", FIELD_FORM );
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

   /* Get the current time/date and write it to a field named TIME_DATE. */

    OSCurrentTIMEDATE(&timedate);

    if ( (nErr = NSFItemSetTime (hNote, "TIME_DATE", &timedate)) != NOERROR )
    {
        printf( "Error: unable to set time in item 'TIME_DATE'.\n" );
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

   /* Create the CompoundText context. We get an Item Context (as opposed
      to a stand-alone context) by specifying the handle to an open note 
      and the name of a rich text field.
    */

    nErr = CompoundTextCreate (
                hNote,        /* handle of note */
                szItemName_RICH_TEXT,  /* item name */
                &hCompound);  /* returned handle to CompoundText context */

    if (nErr != NOERROR)
    {
        printf( "Error: unable to create CompoundText context for 'RICH_TEXT'.\n" );
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

    /* Add the data from the CD file to the CompoundText context.*/

    nErr = CompoundTextAssimilateFile (
                hCompound,          /* handle to CompoundText context */
                CD_FILENAME,       /* file of CD Records */
                0L);                /* reserved */

    if (nErr != NOERROR)
    {
        printf("Error: unable to assimilate CD record from file '%s'.\n",
                    CD_FILENAME);
        CompoundTextDiscard (hCompound);
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

   /* Add the CompoundText context to the note. Since this is an Item 
      Context, (associated with the newly created note), only specify 
      hCompound parameter. 
    */

    nErr = CompoundTextClose (
                hCompound,            /* handle to CompoundText context */
                NULL,                    
                NULL,
                NULL,
                0);

    if (nErr != NOERROR)
    {
        printf("Error: unable to close compound text context.\n");
        CompoundTextDiscard (hCompound);
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

   /* Add the new note to the database. */

    if ( (nErr = NSFNoteUpdate (hNote, 0)) != NOERROR )
    {
        printf("Error: unable to update new note to the database.\n");
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }
   /* Close the new note. */

    if ( (nErr = NSFNoteClose (hNote)) != NOERROR )
    {
        NSFDbClose (hDB);
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
    }

    printf("New note successfully created.\n");

   /* Close the database */

    if ( (nErr = NSFDbClose (hDB)) != NOERROR )
	{
        PrintAPIError (nErr);  
        NotesTerm();
        return (1);
	}

   /* End of subroutine. */
    printf("\nProgram completed successfully\n");
    NotesTerm();
    return (0); 

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
