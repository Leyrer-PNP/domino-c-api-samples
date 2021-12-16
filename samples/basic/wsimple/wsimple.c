/*************************************************************************

    PROGRAM:   wsimple

    FILE:      wsimple.c

    PURPOSE:   Shows how to create a new data note and fill in fields with
               the simple data types.

    SYNTAX:    wsimple

    COMMENTS:
        This program opens the specified database, creates a new data note,
        and fills in fields with the simple data types.

*************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */
#include <stdio.h>

/* Notes API include files */
#if defined (OS390)
#include <lapicinc.h>
#endif

#include <global.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <ostime.h>
#include <osmisc.h>

/* Local function prototypes */
void PrintAPIError (STATUS);

/* Notes API subroutine */
int main (int argc, char *argv[])
{
    char        path_name[]="test.nsf";   /* pathname of database */
    DBHANDLE    db_handle;          /* database handle */
    NOTEHANDLE  note_handle;        /* note handle */
    NUMBER      num_field;          /* contents of a numeric field */
    TIMEDATE    timedate;           /* contents of a time/date field */
    STATUS      error = NOERROR;    /* return code from API calls */

    if (error = NotesInitExtended (argc, argv))
	 {
        printf("\n Unable to initialize Notes.\n");
        return (1);
	 }

  /* Open the database. */

    if (error = NSFDbOpen (path_name, &db_handle))
    {
        PrintAPIError (error);
        NotesTerm();
        return (1);
    }


    /* Create a new data note. */

    if (error = NSFNoteCreate (db_handle, &note_handle))
    {
        PrintAPIError (error);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Write a field named FORM to the note -- this field specifies the
       default form to use when displaying the note. */

    if (error = NSFItemSetText ( note_handle,
                "FORM",
                "SimpleDataForm",
                MAXWORD))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Write a text field named PLAIN_TEXT to the note. */

    if (error = NSFItemSetText ( note_handle,
                "PLAIN_TEXT",
                "The quick brown fox jumped over the lazy dogs.",
                MAXWORD))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Write a field named NUMBER to the note. */

    num_field = 125.007;

    if (error = NSFItemSetNumber (note_handle, "NUMBER", &num_field))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Get the current time/date and write it to a field named TIME_DATE. */

    OSCurrentTIMEDATE(&timedate);

    if (error = NSFItemSetTime (note_handle, "TIME_DATE", &timedate))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Create a text-list field and add it to the note. */

    if (error = NSFItemCreateTextList ( note_handle,
                       "TEXT_LIST",
                       "Charles",
                       MAXWORD))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Add several items to the text-list field. */

    if (error = NSFItemAppendTextList ( note_handle,
                       "TEXT_LIST",
                       "Janet",
                       MAXWORD,
                       TRUE))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    if (error = NSFItemAppendTextList ( note_handle,
                       "TEXT_LIST",
                       "Chuck",
                       MAXWORD,
                       TRUE))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Add the entire new note (with all fields) to the database. */

    if (error = NSFNoteUpdate (note_handle, 0))
    {
        PrintAPIError (error);
        NSFNoteClose (note_handle);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Close the note. (Remove its structure from memory.) */

    if (error = NSFNoteClose (note_handle))
    {
        PrintAPIError (error);
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }

    /* Close the database */

    if (error = NSFDbClose (db_handle))
    {
        PrintAPIError (error);
        NotesTerm();
        return (1);
    }

    /* End of subroutine. */
    printf("\nProgram completed successfully.\n");
    NotesTerm();
    return (0);
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

