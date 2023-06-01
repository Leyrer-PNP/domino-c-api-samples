/****************************************************************************
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

    PROGRAM:    idtables

    FILE:       idtables.c

    SYNTAX:     idtables  -Sdb_name -action [-Bbegin (n) days ago]
                    [-Ncategory] [-Aarchive_name]

    PURPOSE:    Demonstrates how to use the ID table functions

    DESCRIPTION:
        Example of NSF & ID Table routines.

        Given an existing database, time-span, and/or category, either
        delete data notes, touch data notes, or recategorize notes.

****************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif


/* C include files */

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>

/* HCL C API for Notes/Domino include files */

#include     <lapicinc.h>

#include    <global.h>
#include    <nsf.h>
#include    <names.h>
#include    <ostime.h>
#include    <osmem.h>
#include    <idtable.h>
#include    <intl.h>

#include     <lapiplat.h>

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif

/* Local CleanUp defines */
#define DO_NOTHING          0x0000
#define CLOSE_SRC_DB        0x0001
#define CLOSE_NOTE          0x0002
#define UNLOCK_IDTABLE      0x0004
#define FREE_IDTABLE        0x0008
#define CLOSE_ARC_DB        0x0010
#define UNLOCK_ARCTABLE     0x0020
#define FREE_ARCTABLE       0x0040
#define FREE_CPYTABLE       0x0080
#define UNLOCK_CPYTABLE     0x0100
#define FREE_DELTABLE       0x0200
#define UNLOCK_DELTABLE     0x0400

#define TOUCH_NOTES         0x01
#define DELETE_NOTES        0x02
#define STAMP_NOTES         0x04
#define SCAN_BACK           0x08
#define TABLE_DIFFERENCE    0x10

#define HAVE_BEGIN          0x01
#define HAVE_CATEGORY       0x04
#define HAVE_ARCHIVE        0x08

#define CATEGORY_ITEM_NAME "Category"
#define LEN_DATE_STRING       8
#define LINEOTEXT             256
#define MAXOPTIMALDELETIONS   50

void               PrintUsage(void);
STATUS LNPUBLIC  TouchNotes(void *Parameter, DWORD id);
BOOL LNPUBLIC IDScanBackTest(DHANDLE* hTable, DWORD id);
BOOL LNPUBLIC IDTableDifferenceTest(DHANDLE* hTableSrc, DHANDLE* hTableDst);
void PrintAPIError (STATUS);

/************************************************************************

    FUNCTION:   Main

    ALGORITHM:
        1) Assign the parameters of the commandline to variables.
        2) Open the requisite database(s), set the dates for processing
        and display purposes.
        3) Create the ID table of documents to process, its handle and a
        pointer to the ID table. Display statistics of how many there
        are and the table flags. Lock the memory.
        4) Loop over each action parameter to be performed.
        5) Cleanup and exit.

*************************************************************************/

int main(int argc, char *argv[])
{
   STATUS      error = NOERROR,
               cleanup_error = NOERROR;
   DBHANDLE    db_handle_src,
               db_handle_arc;
   WORD        cleanup_state = DO_NOTHING,
               string_len,
               arc_note_class;
   TIMEDATE    data_td_src,
               current_td,
               temp_td,
               nondata_td_src;
   char        *temp_ptr,
               src_name[MAXPATH],
               text_item_value[LINEOTEXT],
               temp_str[LINEOTEXT],
               begin_str[MAXALPHATIMEDATE + 1],
               end_str[MAXALPHATIMEDATE + 1],
               arc_name[MAXPATH];
   DHANDLE       idtable_handle,
               arctable_handle,
               deltable_handle,
               cpytable_handle,
               thetable_handle;
   void        *idtable_ptr,
               *cpytable_ptr;
   NOTEID      note_id,
               arc_note_id;
   DWORD       notes_scanned,
               num_entries,
               text_item_size,
               del_max,
               num_deletions,
               num_invalid,
               del_count;
   WORD        item_type;
   BOOL        ok_flag;
   WORD        action_state = DO_NOTHING,
               dataset_state = DO_NOTHING;
   int         arg_count,
               days;
   TIMEDATE    begin_td,
               end_td,
               idtable_td;
   TFMT        td_format;
   INTLFORMAT     intl_format;
   DBREPLICAINFO  replica_info_src,
                  replica_info_arc;
   DBID           database_id_src,
                  database_id_arc;

/*
 * Assign the parameters of the commandline to the variables of this program.
 */

    error = NotesInitExtended (argc, argv);
    if (error)
    {
       printf("Error: Unable to initialize Notes.\n");
        return (1);
    }

   /* Check arg list for database name, action, start, end and catetory   */
   if (argc < 3 || argc > 7)
   {
       PrintUsage();
         NotesTerm();
         return (0);
   }

   /* Get OS information for string parsing */
   OSGetIntlSettings(&intl_format, sizeof(intl_format));

   /* Spec Time/Date string format for use with convert functions  */
   td_format.Date = TDFMT_FULL;   /* show year, month, and day     */
   td_format.Time = TTFMT_FULL;   /* show hour, minute, and second */
   td_format.Zone = TZFMT_ALWAYS; /* TZ- show on at all times      */

   td_format.Structure = TSFMT_DATETIME; /* req'd. Overall Time/   */

                                         /* Date structure         */

   /* Process command line */
   for (arg_count = 1; arg_count < argc; arg_count++)
   {
      strcpy(temp_str, argv[arg_count]);
      if (temp_str[0] != '-')
      {
         PrintUsage();
         NotesTerm();
           return (0);
      }
      switch (temp_str[1])
      {
         case 'A':
         case 'a':
            strcpy(arc_name, &temp_str[2]);
            dataset_state += HAVE_ARCHIVE;
            break;

         case 'B':
         case 'b':
            temp_ptr = &temp_str[2];
            if((days = atoi(temp_ptr)) < 0)
            {
               PrintUsage();
               goto Exit;
            }
            OSCurrentTIMEDATE(&begin_td);
            TimeDateAdjust(&begin_td,0,0,0,-days,0,0);
            dataset_state += HAVE_BEGIN;
            break;

         case 'C':
         case 'c':
            action_state = STAMP_NOTES;
            break;

         case 'D':
         case 'd':
            action_state = DELETE_NOTES;
            break;

         case 'N':
         case 'n':
            item_type = TYPE_TEXT;
            memcpy(text_item_value, &item_type, sizeof(WORD));
            strcpy(&text_item_value[sizeof(WORD)], &temp_str[2]);
            text_item_size = (DWORD) (strlen(&temp_str[2]) + sizeof(WORD));
            dataset_state += HAVE_CATEGORY;
            break;

         case 'T':
         case 't':
            action_state = TOUCH_NOTES;
            break;

         case 'S':
         case 's':
            strcpy(src_name, &temp_str[2]);
            break;

         case 'F':
         case 'f':
             action_state = TABLE_DIFFERENCE;
             break;

         case 'K':
         case 'k':
             action_state = SCAN_BACK;
             break;

         default:
            PrintUsage();
                NotesTerm();
                return (0);
      }
   }

/*
 Open the requisite database(s), set dates for processing and
 display purposes.
*/
   /* Checkout and use begin_td and end_td during open */

   /* Open source database */
   if (dataset_state & HAVE_BEGIN)
   {

      if (error = NSFDbOpenExtended(
            src_name,
            DBOPEN_WITH_SCAN_LOCK,
            NULLHANDLE,
            &begin_td,
            &db_handle_src,
            &data_td_src,
            &nondata_td_src))
      {
         /* If there is an error, the database is never opened */
         if (ERR(error) == ERR_NO_MODIFIED_NOTES)
         {
            if (error = ConvertTIMEDATEToText(
               &intl_format,
               &td_format,
               &begin_td,
               begin_str,
               MAXALPHATIMEDATE,
               &string_len))
                   goto Exit;
            begin_str[string_len] = '\0';
            printf("\n%s hasn\'t been modified since ", src_name);
            printf(" Begin date: %s\n", begin_str);
               goto Exit;
         }
            else
               goto Exit;
      }
      cleanup_state += CLOSE_SRC_DB;
   }
   else
   {
      if (error = NSFDbOpenExtended(
                     src_name,
                     DBOPEN_WITH_SCAN_LOCK,
                     NULLHANDLE,
                     NULL,
                     &db_handle_src,
                     &data_td_src,
                     &nondata_td_src))
                        goto Exit;
      else
         cleanup_state += CLOSE_SRC_DB;
   }

   /* Now that you have an open database, flesh out TIMEDATE(s) */
      OSCurrentTIMEDATE(&end_td);


   if (error = ConvertTIMEDATEToText(
                  &intl_format,
                  &td_format,
                  &end_td,
                  end_str,
                  MAXALPHATIMEDATE,
                  &string_len))
                     goto Exit;
   end_str[string_len] = '\0';


   if (!(dataset_state & HAVE_BEGIN))
   {
      begin_str[0]   = '\0';

      /* start at beginning of time! */
      TimeConstant (TIMEDATE_WILDCARD, &begin_td);

      dataset_state += HAVE_BEGIN;
   }
   else
   {
      if (error = ConvertTIMEDATEToText(
                     &intl_format,
                     &td_format,
                     &begin_td,
                     begin_str,
                     MAXALPHATIMEDATE,
                     &string_len))
                        goto Exit;
      begin_str[string_len] = '\0';
   }


   /* Just a sanity check */
   if (TimeDateCompare(&begin_td, &end_td) > 0)
   {
      printf("\nResulting Begin time is later than ");
      printf("resulting End time, re-enter\n");
      goto Exit;
   }

/*
   Create the ID table of documents to process, its handle and a pointer to the
   ID table. Display statistics of how many there are and the table flags.
   Lock the memory.
*/
   /* Get the requested documents */
   if (error = NSFDbGetModifiedNoteTable(
                  db_handle_src,
                  NOTE_CLASS_DOCUMENT,
                  begin_td,
                  &end_td,
                  &idtable_handle))
   {
      if (ERR(error) == ERR_NO_MODIFIED_NOTES)
      {
         printf("\n%s contains no data documents from:\n", src_name);
         if (strlen(begin_str))
            printf("\tBegin date: %s\n", begin_str);
         else
            printf("\tBegin date: Beginning of Cosmos\n");
         printf("\tthrough End date: %s\n", end_str);
         goto Exit;
      }
      else
         goto Exit;
   }
   else
   {
      printf("\nYou have selected documents in %s from:\n", src_name);
      if (strlen(begin_str))
         printf("\tBegin date: %s through\n", begin_str);
      else
         printf("\tBegin date: Beginning of Cosmos through\n");
      printf("\tEnd date  : %s\n", end_str);
      cleanup_state += FREE_IDTABLE;
   }

   /* Some ID Table stats */
   if ((idtable_ptr = OSLock(void, idtable_handle)) == NULL)
   {
      printf("\nError: Unable to Lock idtable_handle\n");
      goto Exit;
   }
   else
      cleanup_state += UNLOCK_IDTABLE;

   /* Table Flags and Size */
   printf("\nID Table Flags = %04X\n", IDTableFlags(idtable_ptr));

   if (IDTableFlags(idtable_ptr) == IDTABLE_MODIFIED)
      /* Well we didn't,so NSFDbGetModifiedNoteTable() must have */
      /* Will clear modified flag for our own puposes            */
      IDTableSetFlags(idtable_ptr, 0x0000);

   printf("ID Table size:  %lu\n",IDTableSize(idtable_handle));

   /* Number of entries in the idtable */
   printf("Number of entries in ID Table = %lu",IDEntries(idtable_handle));

   /* Store the current timedate in the structure for later use */
   OSCurrentTIMEDATE(&current_td); /* Get current Time/Date values */
   IDTableSetTime(idtable_ptr, current_td);

   cleanup_state -= UNLOCK_IDTABLE;
   OSUnlock(idtable_handle);

/*
   This is where the real work gets done.
   Each action parameter is performed in this while loop.
*/
   switch (action_state)
   {
      case STAMP_NOTES:
         printf(" will be considered for stamping.\n");
         if (!(dataset_state & HAVE_CATEGORY))
         {
            printf("You must supply a category for action: -C\n");
            goto Exit;
         }


         if ( error = NSFDbStampNotes(
                        db_handle_src,
                        idtable_handle,
                        CATEGORY_ITEM_NAME,
                        (WORD)strlen(CATEGORY_ITEM_NAME),
                        text_item_value,
                        (WORD) text_item_size))
               goto Exit;

         /* Some demographics */
         num_deletions = notes_scanned = 0L;

         while(IDScan(idtable_handle,
                     (FLAG)(notes_scanned++==0L),
                     &note_id))
         {
            if (RRV_DELETED & note_id) num_deletions++;
         }

         notes_scanned--;
         num_entries = IDEntries(idtable_handle);
         printf("\nAnalysis of documents between begin & end:\n");
         printf("\t# of entries in ID Table  = %lu\n", notes_scanned);
         printf("\t# of deletion stubs       = %lu\n", num_deletions);
         printf("\t# of stamped documents    = %lu\n",
            notes_scanned - num_deletions);
         break;

      case TOUCH_NOTES: /* Make copy of idtable for demo purposes */
         printf(" will be considered for touching.\n");

         if ( error = IDTableCopy(idtable_handle, &cpytable_handle))
             goto Exit;
         else
             cleanup_state += FREE_CPYTABLE;
         /* Performance can be enhanced by performing an         */
         /* OSLockObject on cpytable_handle before calling IDEnumerate. */


         if (error = IDEnumerate(cpytable_handle,TouchNotes,&db_handle_src))
             goto Exit;
         /* If you've locked cpytable_handle, be sure to perform */
         /* an OSUnlock on cpytable_handle before proceeding any */
         /* further. Also perform one as required in any error   */
         /* handling you may construct.                          */

         /* Try to get some stats on the cpytable */
         if ((cpytable_ptr = OSLock(void, cpytable_handle)) == NULL)
         {
            printf("\nError:Unable to Lock cpytable_handle\n");
            goto Exit;
         }
         else
             cleanup_state += UNLOCK_CPYTABLE;
            
         temp_td = IDTableTime(cpytable_ptr);

         if (temp_td.Innards[0] > 0L
             && temp_td.Innards[1] > 0L)
         {
            if (error = ConvertTIMEDATEToText(&intl_format, &td_format,
               &temp_td, temp_str,MAXALPHATIMEDATE, &string_len))
                  goto Exit;

            temp_str[string_len] = '\0';
            printf("Modified Time/Date = %s\n", temp_str);
         }

         cleanup_state -= UNLOCK_CPYTABLE;
         OSUnlock(cpytable_handle);

         /* Demo using one table to cleanup another - safe way */
         num_deletions = notes_scanned = 0L;
         while(IDScan(cpytable_handle,
                      (FLAG)(notes_scanned++==0L), &note_id))
         {
            if (IDIsPresent(idtable_handle, note_id))
            {
               if (RRV_DELETED & note_id)
                  num_deletions++;

               error = IDDelete(idtable_handle, note_id, &ok_flag);
               if (!ok_flag || error)
               {
                  printf("\nIDDelete from idtable failed.\n");
                  goto Exit;
               }
            }
         }

         /* Some demographics */
         notes_scanned--;


         num_entries = IDEntries(cpytable_handle);
         printf("\nAnalysis of documents between begin & end:\n");
         printf("\t# of entries in ID Table  = %lu\n", notes_scanned);
         printf("\t# of deletion stubs       = %lu\n", num_deletions);
         printf("\t# of touched documents    = %lu\n",
                 notes_scanned - num_deletions);



         /* Now zap the original - quick and dirty way */
         if (error = IDDeleteAll(cpytable_handle))
                     goto Exit;


         cleanup_state -= FREE_CPYTABLE;
         IDDestroyTable(cpytable_handle);

         break;

      case DELETE_NOTES: /* Create the archive table the hard way */
         printf(" will be considered for deletion.\n");
         if ((idtable_ptr = OSLock(void, idtable_handle)) == NULL)
         {
            printf("\nError:Unable to Lock idtable_handle\n");
            goto Exit;
         }
         else
            cleanup_state += UNLOCK_IDTABLE;

         idtable_td = IDTableTime(idtable_ptr);

         /* Using 0 for the Alignment parameter of IDCreateTable will
            cause Domino and Notes to figure out the proper Alignment
            value to use */
         if (error = IDCreateTable(0, &arctable_handle))
            goto Exit;
         else
            cleanup_state += FREE_ARCTABLE;

         cleanup_state -= UNLOCK_IDTABLE;
         OSUnlock(idtable_handle);


         if (dataset_state & HAVE_ARCHIVE)
         {
            /* Open the Archival Database */
            if (error = NSFDbOpenExtended(
                  arc_name,
                  DBOPEN_WITH_SCAN_LOCK,
                  NULLHANDLE,
                  NULL,
                  &db_handle_arc,
                  NULL,
                  NULL))
                     goto Exit;
            else
               cleanup_state += CLOSE_ARC_DB;

            /* Get the Database IDs */
            if (error = NSFDbIDGet(
                           db_handle_src,
                           &database_id_src))
                              goto Exit;
            if (error = NSFDbIDGet(
                           db_handle_arc,
                           &database_id_arc))
                              goto Exit;

            /* Get the Database replica information */
            if (error = NSFDbReplicaInfoGet(
                           db_handle_src,
                           &replica_info_src))
                              goto Exit;
            if (error = NSFDbReplicaInfoGet(
                           db_handle_arc,
                           &replica_info_arc))
                              goto Exit;

            /* Populate the newly created archive table */
            num_invalid = num_deletions = notes_scanned = 0L;
            while(IDScan(
               idtable_handle,
               (FLAG)(notes_scanned++==0L),
               &note_id))
            {
               /* Check returned class for deletion stubs */
               if (note_id & RRV_DELETED)
                  num_deletions++;
               else
               {
                  error = IDInsert(
                              arctable_handle,
                              note_id,
                              &ok_flag);
                  if (!ok_flag || error)
                  {
                     printf("\nIDInsert in arctable failed.\n");
                     goto Exit;
                  }
               }
            }  /* end of while */

            /* Adjust for extra counting during while terminate */
            notes_scanned--;

            /* Some demographics */
            num_entries = IDEntries(arctable_handle);
            printf("\nAnalysis of documents between begin & end:\n");
            printf("\t# of entries in ID Table  = %lu\n", notes_scanned);
            printf("\t# of entries in arc table = %lu\n", num_entries);
            printf("\t# of deletion stubs       = %lu\n", num_deletions);
            printf("\t# of invalid documents    = %lu\n", num_invalid);

            /* Perform Db note copy & demo manual wipe out the idtable.*/
            notes_scanned = 0L;
            while(IDScan(
                     arctable_handle,
                     (FLAG)(notes_scanned++==0L),
                     &note_id))
            {
               /* Copy real live data notes to the archive Db */
               if ( error = NSFDbCopyNote(
                             db_handle_src,
                             NULL,
                             &replica_info_src.ID,
                             note_id,
                             db_handle_arc,
                             &database_id_arc,
                             &replica_info_arc.ID,
                             &arc_note_id,
                             &arc_note_class ))
               {
                  if (ERR(error) == ERR_INVALID_NOTE)
                  {
                     error = NOERROR;
                     printf("#%lu: Note ID %08lX in %s ",
                        notes_scanned, arc_note_id,arc_name);
                     printf(" is an Invalid Document!\n");
                     continue;
                  }
                  else
                     goto Exit;
               }

               if (arc_note_class & SEARCH_NOTIFYDELETIONS)
               {
                  printf("#%lu: Note ID %08lX in %s ",
                           notes_scanned, arc_note_id, arc_name);
                  printf(" is a deletion stub!\n");
               }
            }

            /* Adjust notes_scanned for while loop terminate */
            notes_scanned--;
            printf("\nArchived %lu documents in %s\n", notes_scanned,arc_name);

            cleanup_state -= CLOSE_ARC_DB;
            if (error = NSFDbClose(db_handle_arc))
               goto Exit;

            thetable_handle = arctable_handle;
         }
         else
             thetable_handle = idtable_handle;

         /* Batch any Deletions */
         if (error = IDCreateTable(0, &deltable_handle))
             goto Exit;
         else
             cleanup_state += FREE_DELTABLE;

         del_max = IDEntries(thetable_handle);
         del_count = 0L;
         while( del_count < del_max )
         {
            while (IDScan(
                     thetable_handle,
                     (FLAG)(del_count++==0L),
                     &note_id))
            {
               /* Build a table to batch operation */
               error = IDInsert(
                     deltable_handle,
                     note_id,
                     &ok_flag);

               if (!ok_flag || error)
               {
                  printf("IDInsert in deltable failed.\n");
                  goto Exit;
               }
               if ((del_count % MAXOPTIMALDELETIONS) == 0)
                  break;
            }
            if (IDEntries(deltable_handle))
            {   /* Using NULL in 3rd arg avoids ERR_REMOTE_UNID */

               if (error = NSFDbDeleteNotes(
                              db_handle_src,
                              deltable_handle,
                              NULL))
                  goto Exit;

               /* adjust del_count exit of while(IDScan) */
               printf("\n%s %lu documents in %s\n",
                  (del_count <= del_max)? "Deleting" : "Deleted",
                  (del_count <= del_max)? 50L : del_count-1L, src_name);


               if (error = IDDeleteAll(deltable_handle))
                  goto Exit;
            }
         }

         cleanup_state -= FREE_DELTABLE;
         IDDestroyTable(deltable_handle);

         break;

         cleanup_state -= FREE_ARCTABLE;
         IDDestroyTable(arctable_handle);

         break;

    case SCAN_BACK:
    {
         BOOL retResult;
         /* noteid to be inserted */
         DWORD dwNoteid = 0x8120;
         printf(" \n Testing Scan Back \n");
         if (error = IDTableCopy(idtable_handle, &cpytable_handle))
             goto Exit;
         else
             cleanup_state += FREE_CPYTABLE;
         /* Adding note id into table*/
         if (error = IDInsert(cpytable_handle, dwNoteid, &retResult))
         {
             PrintAPIError(error);
             goto Exit;
         }
         if (error != NOERROR)
         {
             PrintAPIError(error);
             goto Exit;
         }
         else
         {
             if (error = IDScanBackTest(&cpytable_handle, dwNoteid))
                 if (error != NOERROR)
                 {
                     PrintAPIError(error);
                     goto Exit;
                 }
         }
         break;
     }
     case TABLE_DIFFERENCE:
     {
         DHANDLE hTableDst;
         BOOL retResult;
         /* noteid to be inserted */
         DWORD dwNoteid = 0x8110; 

         printf("\n Testing Table differences \n");

         if (error = IDTableCopy(idtable_handle, &cpytable_handle)) {

             goto Exit;
         }
         cleanup_state += FREE_CPYTABLE;

         if (error = IDTableCopy(idtable_handle, &hTableDst))
             goto Exit;
         /* Adding new noteid into the table */
         if (error = IDInsert(hTableDst, dwNoteid, &retResult))
         {
             IDDestroyTable(hTableDst);
             goto Exit;
         }
         if (IDTableDifferenceTest(&cpytable_handle, &hTableDst))
         {
             printf("\n Table difference test is successful.\n");
             IDDestroyTable(hTableDst); 
         }
         else
         {
             IDDestroyTable(hTableDst); 
             goto Exit;
         }


         break;

     }
        default:
            break;
    }

/*
   Cleanup and exit.
 */
   /* Normal cleanup, common to all actions */
   cleanup_state -= FREE_IDTABLE;
   IDDestroyTable(idtable_handle);


   cleanup_state -= CLOSE_SRC_DB;
   if (error = NSFDbClose(db_handle_src))
      goto Exit;

    printf("\nAll Done!\n");

    NotesTerm();

   printf("\nProgram completed successfully.\n");

    return(0);

Exit:   /* ERROR HANDLING */

   if (cleanup_state)
   {
      if (cleanup_state & FREE_CPYTABLE)
         IDDestroyTable(cpytable_handle);

      if (cleanup_state & FREE_ARCTABLE)
         IDDestroyTable(arctable_handle);

      if (cleanup_state & FREE_DELTABLE)
         IDDestroyTable(deltable_handle);

      if (cleanup_state & UNLOCK_IDTABLE)
         OSUnlockObject(idtable_handle);

      if (cleanup_state & FREE_IDTABLE)
         IDDestroyTable(idtable_handle);

      if (cleanup_state & CLOSE_ARC_DB)
         if (cleanup_error = NSFDbClose(db_handle_arc))
            error = cleanup_error;

      if (cleanup_state & CLOSE_SRC_DB)
         if (cleanup_error = NSFDbClose(db_handle_src))
            error = cleanup_error;
   }
   PrintAPIError(error);
   NotesTerm();
    return(1);
}

BOOL LNPUBLIC IDScanBackTest(DHANDLE* hTable, DWORD id)
{
    STATUS error = NOERROR;
    int alignment = 4;
    BOOL bresult = FALSE;
    int i;

    printf("ID = %d", id);
    /* checking the given is last entry in table */
    if (!(IDIsPresent(*hTable, id)))
    {
        printf("\n ID doesn't exist");
        return (FALSE);
    }
    /* fetches previous entry since flag is false */
    if (IDScanBack(*hTable, FALSE, &id))
    {
        printf("\n ID present after %d", id);
        return (TRUE);
    }
    else
        return(FALSE);

    return TRUE;
}

BOOL LNPUBLIC IDTableDifferenceTest(DHANDLE* hTableSrc, DHANDLE* hTableDst)
{
    STATUS error = NOERROR;
    NOTEID NoteId;
    DHANDLE hDelete = NULLHANDLE;
    DHANDLE hAdd = NULLHANDLE;
    DHANDLE hSame = NULLHANDLE;

    /* destination cannot be null source can be null */
    if (NULLHANDLE == *hTableDst)
        return FALSE;



    error = IDTableDifferences(*hTableSrc, *hTableDst, &hAdd
        , &hDelete, &hSame);

    if (NOERROR != error)
    {
        PrintAPIError(error);
        return FALSE;
    }


    if (NULLHANDLE != hAdd)
    {
        if (IDEntries(hAdd) > 0)
        {
            printf("\n There are additions to make table1 same as table2 \n");
            printf(" No. of added entries  = %ld", IDEntries(hAdd));
        }
        IDDestroyTable(hAdd);
    }
    else if (NULLHANDLE != hDelete)
    {
        if (IDEntries(hDelete) > 0)
        {
            printf("\n There are deletions to make table1 same as table2 \n");
            printf(" No. of deleted entries  = %ld", IDEntries(hDelete));
        }
        IDDestroyTable(hDelete);
    }
    else if (NULLHANDLE != hSame) 
    {
        printf("\n There are no changes as both tables are the same \n");
        IDDestroyTable(hSame);
    }

    return TRUE;

}

STATUS LNPUBLIC  TouchNotes(void *Parameter, DWORD id)
{
   STATUS  error = NOERROR, cleanup_error = NOERROR;
   WORD    cleanup_state = DO_NOTHING;
   DHANDLE  note_handle;

   /* Is it a deletion stub? */
   if (id & RRV_DELETED)
      return (NOERROR);

   /* Open the Note you wish to touch */
   if (error = NSFNoteOpen(
                  *((DBHANDLE *) Parameter),
                  id,
                  OPEN_EXPAND,
                  &note_handle))
                     goto Exit;
   else
       cleanup_state += CLOSE_NOTE;

   /* Changes the LastModified TIMEDATE in the Note's header */
   if (error = NSFNoteUpdate(
                  note_handle,
                  UPDATE_NOCOMMIT))
                     goto Exit;
   cleanup_state -= CLOSE_NOTE;
   if (error = NSFNoteClose(note_handle))
      goto Exit;

Exit:   /* ERROR HANDLING */
   if (cleanup_state & CLOSE_NOTE)
      if (cleanup_error = NSFNoteClose(note_handle))
         error = cleanup_error;

   return (error); /* return latest error */
}

/* Display message on how to run this program.  */
void PrintUsage(void)
{
   printf("\nUSAGE:\n\n");
   printf("idtables -Sdb_name -action [-Bbegin (n) days ago]");
   printf(" [-Ncategory]");
   printf(" [-Aarchive_name]\n\n");
   printf("\taction:\n");
   printf("\t\tT  touch selected document\'s last modified date.\n");
   printf("\t\tD  delete selected documents.\n");
   printf("\t\tC  stamp selected documents with a given category value.\n");
   printf("\toptions:\n");
   printf("\t\tB  begin document selection (n) days ago.\n");
   printf("\t\tN  new category string. Use with 'C' action.\n");
   printf("\t\tA  database to act as archive for deleted documents.\n");
   return;
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

