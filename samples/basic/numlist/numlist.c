/****************************************************************************
    PROGRAM:    numlist

    FILE:       numlist.c

    SYNTAX:     numlist  <database filename>
                          or 
                numlist

    PURPOSE:    Create a number list field 

    DESCRIPTION:
        Open the specified database. Create a document. Set Form field   
        to "SimpleDataForm".  Set PLAIN_TEXT field. Set NUMBER field to  
        the a list of the following five values: 1,3,9703.4,-7,0.11592.  
        Update and close the document. Close the database.               
                                                                         
****************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Notes API include files */

#include <global.h>
#include <nsfdb.h>
#include <nsfdata.h>
#include <nsfnote.h>
#include <misc.h>
#include <ods.h>
#include <editods.h>
#include <editdflt.h>
#include <fontid.h>

#include <osmisc.h>
              
void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                         char *db_filename); 
                         
void PrintAPIError (STATUS);

#define  STRING_LENGTH  256
              
STATUS LNPUBLIC AppendNumberListField (
                    NOTEHANDLE hNote,
                    const char * szFieldName,
                    NUMBER * aNumbers,
                    WORD  usCount);

/************************************************************************

    FUNCTION:   main

************************************************************************/

int main(int argc, char *argv[])
{
#define LIST_COUNT 5
    char        *szPathName;
    DBHANDLE    hDB;
    NOTEHANDLE  hNote;
    STATUS      error = NOERROR;
    NUMBER      aNumbers[LIST_COUNT] = {1,3,9703.4,-7,0.11592};
    char        database_name[STRING_LENGTH];
   
    szPathName = database_name;
    ProcessArgs(argc, argv, szPathName); 
   
    if (error = NotesInitExtended (argc, argv))
	{
        printf("\n Unable to initialize Notes.\n");
        return (1);
	}


/* Open the database. */

    if (error = NSFDbOpen (szPathName, &hDB))
    {
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    } 
        
    printf("\nOpened database: \"%s\"\n", szPathName); 
    fflush(stdout);

/* Create a new data note. */

    if (error = NSFNoteCreate (hDB, &hNote))
    {
        NSFDbClose (hDB);
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    }

/* Write a field named FORM to the note */

    if (error = NSFItemSetText (hNote, "FORM", "SimpleDataForm", MAXWORD))
    {
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    }

/* Write a text field named PLAIN_TEXT to the note. */

    if (error = NSFItemSetText ( hNote, "PLAIN_TEXT", 
                    "Created by sample NUMLIST.", MAXWORD))
    {
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    }

/* Write a field named NUMBER to the note. */

        
    printf("Appending number list.\n", szPathName); 
    fflush(stdout);
    if (error = AppendNumberListField (
                     hNote, 
                     "NUMBER", 
                     aNumbers, 
                     LIST_COUNT))
    {
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    }    
        
    printf("Updating and closing note.\n", szPathName); 
    fflush(stdout);

    if (error = NSFNoteUpdate (hNote, 0))
    {
        NSFNoteClose (hNote);
        NSFDbClose (hDB);
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    }
        
    if (error = NSFNoteClose (hNote))
    {
        NSFDbClose (hDB);
        PrintAPIError (error);  
        NotesTerm();
        return (1);
    }

    if (error = NSFDbClose (hDB))
	{
        PrintAPIError (error);  
        NotesTerm();
        return (1);
	}

    printf("\nProgram completed successfully.\n"); 

    NotesTerm();
    return (0); 

}

/************************************************************************

   FUNCTION:   AppendNumberListField

   DESCRIPTION:
      Uses the RANGE data structure to build the item value of the number 
      list. Then calls NSFItemAppend to append the number list to the note. 
                                                                         
************************************************************************/

STATUS LNPUBLIC AppendNumberListField (
                     NOTEHANDLE hNote,
                     const char * szFieldName,
                     NUMBER * aNumbers,
                     WORD  usCount)
{
    DWORD       dwValueLen;
    void       *pvoidItemValue;
    RANGE      *pRange;
    NUMBER     *pNumber;
    WORD        i;
    STATUS      error;
    
    /* Allocate a buffer to hold the RANGE structure followed by
       the NUMBERs in the number list */

    dwValueLen = sizeof(RANGE) + (usCount * sizeof(NUMBER));
    pvoidItemValue = (void *) malloc ((size_t)dwValueLen);
    
    if (pvoidItemValue == NULL)
    {
        printf ("malloc failed\n");
        return (ERR(PKG_NSF+52)); 
    }

    /* Store the RANGE structure in the buffer */

    pRange = (RANGE*)pvoidItemValue;
    pRange->ListEntries = usCount;
    pRange->RangeEntries = 0;
    pRange++;

    /* Store each of the NUMBERs in the buffer.  */
    pNumber = (NUMBER *)pRange;
    for (i = 0; i < usCount; i++)
    {
        memcpy ((void *)pNumber, (void *)(aNumbers + i), sizeof(NUMBER));
        pNumber++;
    }
    
    error = NSFItemAppend (hNote,
                ITEM_SUMMARY,
                szFieldName,
                (WORD) strlen(szFieldName),
                TYPE_NUMBER_RANGE,
                pvoidItemValue,
                dwValueLen);

    free (pvoidItemValue);

    return (ERR(error));
}


/************************************************************************

    FUNCTION:   ProcessArgs

    INPUTS:     argc, argv - directly from the command line
   
    OUTPUTS:    db_filename get data from the command line or from what 
                the user types at a prompt 

*************************************************************************/

void  LNPUBLIC  ProcessArgs (int argc, char *argv[], char *db_filename)
{ 
    if (argc != 2)  
    {

        printf("Enter filename of database: ");      
        fflush (stdout);
        gets(db_filename);

    }    
    else
    {
        strcpy(db_filename, argv[1]);    
    } /* end if */
} /* ProcessArgs */


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

