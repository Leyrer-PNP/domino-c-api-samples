/****************************************************************************
    PROGRAM:    viewids

    FILE:       viewids.c

    SYNTAX:     viewids

    PURPOSE:    Finds all the documents in the view. For each document, 
                get the note ID and print it. 

    DESCRIPTION:
        This sample shows how to open a view note, open the corresponding
        collection (index), and read the entries in the collection. For
        each entry, this reads the note ID. This just prints out the note
        ID, but note that, given the note ID, you could perform any 
        other operation on that note.

****************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* C include files */

#include <stdio.h>
#include <string.h>

/* HCL API for Notes/Domino include files */

#if defined(OS390)
#include <lapicinc.h>
#endif

#include <global.h>
#include <nsfdb.h>
#include <nif.h>
#include <osmem.h>
#include <miscerr.h>
#include <osmisc.h>

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif

/* cleanup flag values */
#define DO_NOTHING       0x0000
#define CLOSE_DB         0x0001
#define CLOSE_COLLECTION 0x0002
#define NAME_LEN 256

void PrintAPIError (STATUS);
void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                               char *dbName, char *viewName);


#ifdef OS390
#include <_Ascii_a.h>                /* NOTE: must be the LAST file included */
#endif

/* Notes API subroutine */

int main ( int argc, char *argv[])
{
   char     DBFilename[NAME_LEN]="";/* pathname of the database */
   char     ViewName[NAME_LEN]="";/* name of the view we'll read */
   DBHANDLE hDB;                    /* handle of the database */
   NOTEID   ViewID;                 /* note id of the view */
   HCOLLECTION hCollection;         /* collection handle */
   COLLECTIONPOSITION CollPosition; /* position within collection */
   DHANDLE    hBuffer;                /* handle to buffer of note ids */
   NOTEID   *IdList;                /* pointer to a note id */
   DWORD    EntriesFound;           /* number of entries found */
   DWORD    NotesFound=0;           /* number of documents found */
   WORD     SignalFlag;             /* signal and share warning flags */
   DWORD    i;                      /* a counter */
   STATUS   error = NOERROR;        /* return status from API calls */
   STATUS   returnCode = NOERROR;   /* supplementary return status */
   WORD     cleanup=DO_NOTHING;

   if (error = NotesInitExtended(argc, argv))
   {
      printf("\n Unable to initialize Notes. Error Code[0x%04x]\n", error);
      return(1);
   }

   /* Processing input arguments */

   memset(DBFilename, '\0', NAME_LEN);
   memset(ViewName, '\0', NAME_LEN);
   ProcessArgs(argc, argv, DBFilename, ViewName);

/* Open the database. */

   if (error = NSFDbOpen (DBFilename, &hDB))
      goto exit1;

   cleanup |= CLOSE_DB;


/* Get the note id of the view we want. */

   if (error = NIFFindView (hDB, ViewName, &ViewID))
      goto exit1;


/* Get a collection using this view. */

   if (error = NIFOpenCollection(
         hDB,            /* handle of db with view */
         hDB,            /* handle of db with data */
         ViewID,         /* note id of the view */
         0,              /* collection open flags */
         NULLHANDLE,     /* handle to unread ID list */
         &hCollection,   /* collection handle */
         NULLHANDLE,     /* handle to open view note */
         NULL,           /* universal note id of view */
         NULLHANDLE,     /* handle to collapsed list */
         NULLHANDLE))    /* handle to selected list */
      goto exit1;

   cleanup |= CLOSE_COLLECTION;

/* Set up the data structure, COLLECTIONPOSITION, that controls where in
the collection we will begin when we read the collection.  Specify that we
want to start at the beginning. */

   CollPosition.Level = 0;
   CollPosition.Tumbler[0] = 0;

/* Get a buffer with information about each entry in the collection. 
   Perform this routine in a loop.  Terminate loop when SignalFlag
   indicates that there is no more information to get.   */

   do
   {
      if ( error = NIFReadEntries(
             hCollection,        /* handle to this collection */
             &CollPosition,      /* where to start in collection */
             NAVIGATE_NEXT,      /* order to use when skipping */
             1L,                 /* number to skip */
             NAVIGATE_NEXT,      /* order to use when reading */
             0xFFFFFFFF,         /* max number to read */
             READ_MASK_NOTEID,   /* info we want */
             &hBuffer,           /* handle to info buffer */
             NULL,               /* length of info buffer */
             NULL,               /* entries skipped */
             &EntriesFound,      /* entries read */
             &SignalFlag))       /* share warning and more signal flag */
         goto exit1;


/* Check to make sure there was a buffer of information returned.
(We would crash if we tried to proceed with a null buffer.) */ 

      if (hBuffer == NULLHANDLE)
      {
         printf ("\nEmpty buffer returned by NIFReadEntries.\n");
         goto exit1;
      }

/* Lock down (freeze the location) of the buffer of entry IDs. Cast
the resulting pointer to the type we need. */

      IdList = (NOTEID *) OSLockObject (hBuffer);

/* Print out the list of note IDs found by this search. Don't print a note 
ID if it is a "dummy" ID that stands for a category in the collection. */

      printf ("\n");
      for (i=0; i<EntriesFound; i++)
      {
         if (NOTEID_CATEGORY & IdList[i]) continue; 
         printf ("Note count is %lu. \t Note ID is: %lX\n", 
                 ++NotesFound, IdList[i]);
      }

/* Unlock the list of IDs. */

      OSUnlockObject (hBuffer);

/* Free the memory allocated by NIFReadEntries. */

      OSMemFree (hBuffer);

/* Loop if the end of the collection has not been reached because the buffer
   was full.  */

   }  while (SignalFlag & SIGNAL_MORE_TO_DO);


exit1:
   if (cleanup * CLOSE_COLLECTION)
      NIFCloseCollection(hCollection);

   if (cleanup * CLOSE_DB)
      NSFDbClose(hDB);

   if (error)
      PrintAPIError(error);

   NotesTerm();

   if (returnCode==NOERROR && error==NOERROR)
      printf("\nProgram completed successfully.\n");

   if (returnCode)
      return(returnCode);
   else
      return(error);
   
}

/************************************************************************

    FUNCTION:   ProcessArgs

    INPUTS:     argc, argv - directly from the command line

    OUTPUTS:    Key1, Key1 - data from the command line or
                from what the user types when prompted.

*************************************************************************/

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                               char *dbName, char *viewName)
{
    if (argc != 3)
    {
        printf("Enter the DB Name: ");
        fflush(stdout);
        fgets(dbName, NAME_LEN-1, stdin);

        printf("Enter the View Name: ");
        fflush(stdout);
        fgets(viewName,  NAME_LEN-1, stdin);

    }
    else
    {
         strncpy(dbName, argv[1], NAME_LEN-1);
         strncpy(viewName, argv[2], NAME_LEN-1);

    } /* end if */
} /* ProcessArgs */


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

