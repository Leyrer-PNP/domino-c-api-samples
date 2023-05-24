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

	PROGRAM:		nif

	FILE:			nif.c

	PURPOSE:
	This program demonstrates how to use the API's
	NIFCollectionUpToDate, NIFIsNoteInView, NIFIsTimeVariantView,
	NIFGetViewRebuildDir and NIFReadEntriesExt.
			
	SYNTAX:
	nif <database filename> <time varying view name> <not a time variant view name>
		or
	nif
	
	Example: nif acls.nsf "simple view" "SimpleDataView"

	Description:
	This program checks whether the view in a database is time-varying,
	the collection is up-to-date, and the note ID is in the view.
	It also gets the last modified time of the database and view rebuild
	directory.

*************************************************************************/

/* C include files. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files. */

#include <global.h>
#include <nsfdb.h>
#include <nsfdata.h>
#include <osmisc.h>
#include <osfile.h>
#include <idtable.h>
#include <lapiplat.h>
#include <nif.h>
#include <osenv.h>
#include <textlist.h>
#include <osmem.h>
#include <misc.h>
#include <miscerr.h>
#include <editods.h>

#define  STRING_LENGTH  256

/* Local function prototypes. */

void  LNPUBLIC  ProcessArgs (int argNumber, char *szArgVector[], char *szDBFileName, char *szTimeVariantView, char *szViewName);
void PrintAPIError (STATUS);
BOOL LNPUBLIC NIFCollectionUpToDateTest(DHANDLE hDB, NOTEID noteid);
void LNPUBLIC NIFIsNoteInViewTest(DHANDLE hDB, NOTEID viewnoteid, NOTEID testnoteid);
BOOL LNPUBLIC NIFIsTimeVariantViewTest(DHANDLE hDB, NOTEID viewnoteid);
BOOL LNPUBLIC NIFReadEntriesExtTest(DHANDLE hDB, NOTEID viewnoteid);
BOOL LNPUBLIC NSFNoteCreateTest(DHANDLE hDB);
BOOL LNPUBLIC NIFGetViewRebuildDirTest();
STATUS PrintSummary (BYTE *);

/************************************************************************
FUNCTION:	main
PURPOSE:	Main HCL C API for Notes/Domino subroutine.
ALGORITHM:
1)	Read or Get the command line arguments and
	parse them into the appropriate variables.  
2)	Open database and get the note ID of the two input views.
3)	Call NIFIsTimeVariantViewTest function to check whether
	the view is time varying or not.
4)	Call NIFReadEntriesExtTest function to read the entries
	in the view and also to get the last updated time of an
	NSF file.
5)	Add a note to the NSF file. Update and close note.
6)	Call NIFCollectionUpToDateTest function to check whether
	the collection is up to date or not. If not, then update
	the NSF file.
7)	Open the collection and check that the note ids are in the 
	view by calling the NIFIsNoteInViewTest function inside the 
	NIFReadEntries call. Then close the collection.
8)	Again, call the NIFReadEntriesExtTest function to check whether
	the note is added and to get the last modified time of the 
	NSF file and call NIFGetViewRebuildDirTest to get view rebuild
	directory.
9)	Close the NSF file.
*************************************************************************/

int main(int argc, char* argv[])
{

	/* Local data declarations. */

	char szDbName[STRING_LENGTH] = {0};          /* Name of database */
	char szViewName[STRING_LENGTH] = {0};        /* Name of view that is not time varying */
	char szTimeVariantView[STRING_LENGTH] = {0}; /* Time Variant view */
	DBHANDLE hDbHandle = NULLHANDLE;             /* database handle */
	HCOLLECTION hCollection = NULLHANDLE;        /* collection handle */
	COLLECTIONPOSITION CollPosition;             /* position within collection */
	DHANDLE hBuffer = NULLHANDLE;                /* handle to buffer of note ids */
	DWORD dwEntriesFound = 0;                    /* number of entries found */
	NOTEID* IdList = NULL;                       /* pointer to a note id */
	WORD wSignalFlag = 0;                        /* signal and share warning flags */
	STATUS error = NOERROR;                      /* error code from API calls */
	NOTEID viewid;                               /* view id from db*/
	NOTEID testid = 0x123;                       /* test if dor notesin view test */
	BOOL bResult = FALSE;                        /* To store the bool output */
	NOTEID TimeVariantViewid;                    /* id of the Time variant view from db*/
	
	ProcessArgs(argc, argv, szDbName, szTimeVariantView, szViewName);
	
	error = NotesInitExtended(argc, argv);

	if (error)
	{
		PrintAPIError(error);
		return (1);
	}

	/* Open the database. */

	if (error = NSFDbOpen (szDbName, &hDbHandle))
	{
		PrintAPIError(error);
		goto Exit1;
	}

	/* Get the note id of the view that is not time varying. */

	if (error = NIFFindView (hDbHandle, szViewName, &viewid))
	{
		PrintAPIError(error);
		goto Exit0;
	}
	
	/* Get the note id of the time variant view. */

	if (error = NIFFindView (hDbHandle, szTimeVariantView, &TimeVariantViewid))
	{
		PrintAPIError(error);
		goto Exit0;
	}

	printf("\nTesting NIFIsTimeVariantViewTest:\n");
	printf("--------------------------------\n");

	/* Checks whether the view is a time varying view or 
	any type of view that is rebuilt on each open. */

	bResult = NIFIsTimeVariantViewTest(hDbHandle, TimeVariantViewid);
	if (bResult == TRUE)
	{
		printf("In %s, the view \"%s\" is a Time Variant View.\n",szDbName, szTimeVariantView);
		printf("NIFIsTimeVariantViewTest was successful.\n\n");
	}
	else
	{
		printf("NIFIsTimeVariantViewTest failed.\n\n");
	}

	printf("\nTesting NIFReadEntriesExtTest:\n");
	printf("-------------------------------\n");	
	printf("Reading NSF file.\n");

	/* Reads the entries and gets the last updated time of the NSF file. */

	if (NIFReadEntriesExtTest (hDbHandle, viewid))
	{
		printf("NIFReadEntriesExtTest was successful.\n\n");
	}
	else
	{
		printf("NIFReadEntriesExtTest failed.\n");
		goto Exit0;
	}
	

	/* Creates a notes document in NSF file. */

	printf("Creating note in NSF file:\n");
	printf("----------------------------\n");
	printf("Adding a note into NSF file.\n");

	if (!NSFNoteCreateTest (hDbHandle))
	{
		goto Exit0;
	}

	/* Checks whether the collection is up to date. */

	if (NIFCollectionUpToDateTest (hDbHandle, viewid))
	{
		printf("NIFCollectionUpToDateTest was successful.\n");
	}
	else
	{
		printf("NIFCollectionUpToDateTest failed.\n");
	}


	/* Get a collection of view that is not time varying. */

	if (error = NIFOpenCollection (
		hDbHandle,				/* handle of db with view */
		hDbHandle,				/* handle of db with data */
		viewid,					/* note id of the view */
		0,						/* collection open flags */
		NULLHANDLE,				/* handle to unread ID list */
		&hCollection,			/* collection handle */
		NULLHANDLE,				/* handle to open view note */
		NULL,					/* universal note id of view */
		NULLHANDLE,				/* handle to collapsed list */
		NULLHANDLE))			/* handle to selected list */
	{
		PrintAPIError (error);
		goto Exit0;
	}

	/* Set up the data structure, COLLECTIONPOSITION, to control
	where to begin reading in the collection. Specify to
	start at the beginning. */

	CollPosition.Level = 0;
	CollPosition.Tumbler[0] = 0;

	do
	{
		if (error = NIFReadEntries (
			hCollection,		/* handle to this collection */
			&CollPosition,		/* where to start in collection */
			NAVIGATE_NEXT,		/* order to use when skipping */
			1L,					/* number to skip */
			NAVIGATE_NEXT,		/* order to use when reading */
			0xFFFFFFFF,			/* max number to read */
			READ_MASK_NOTEID,	/* info we want */
			&hBuffer,			/* handle to info buffer */
			NULL,				/* length of info buffer */
			NULL,				/* entries skipped */
			&dwEntriesFound,	/* entries read */
			&wSignalFlag))		/* share warning and more signal flag */
		{
			NIFCloseCollection(hCollection);
			PrintAPIError (error);
			goto Exit0;
		}

		/* Check to make sure there was a buffer of information returned
		(We would crash if we tried to proceed with a null buffer). */

		if (hBuffer == NULLHANDLE)
		{
			NIFCloseCollection(hCollection);
			printf("\nEmpty buffer returned by NIFReadEntries.\n");
			goto Exit0;
		}

		/* Lock down (freeze the location) of the buffer of entry IDs. Cast
		the resulting pointer to the type we need. */

		IdList = (NOTEID*)OSLockObject(hBuffer);

		/* Print out the list of note IDs found by this search. Don't print a note
		ID if it is a "dummy" ID that stands for a category in the collection. */

		for (int i = 0; i < dwEntriesFound; i++)
		{
			if (NOTEID_CATEGORY & IdList[i])
				continue;
		}

		/* Negative case to test note ID does not exist in the view. */
		NIFIsNoteInViewTest(hDbHandle, viewid, testid);

		/* Positive case to test note ID exist in the view. */
		NIFIsNoteInViewTest(hDbHandle, viewid, IdList[0]);

		/* Unlock the list of IDs. */

		OSUnlockObject(hBuffer);

		/* Free the memory allocated by NIFReadEntries. */

		OSMemFree(hBuffer);

		/* Loop if the end of the collection has not been reached because the buffer was full. */

	} while (wSignalFlag & SIGNAL_MORE_TO_DO);
	
	printf("\nTesting NIFGetViewRebuildDir:\n");
	printf("----------------------------\n");

	if (NIFGetViewRebuildDirTest ())
	{
		printf("NIFGetViewRebuildDir was successful.\n");
	}

	/* Close the collction. */

	if (error = NIFCloseCollection (hCollection))
	{
		PrintAPIError(error);
		goto Exit0;
	}
	
	/* Check NIFReadEntriesExt after adding document to NSF file. */

	printf("\nTesting NIFReadEntriesExtTest after changing NSF file:\n");
	printf("------------------------------------\n");
	printf("Printing the entries after updating NSF file.\n"); 
	
	if (NIFReadEntriesExtTest (hDbHandle, viewid))
	{
		printf("NIFReadEntriesExtTest was successful.\n");
	}
	else
	{
		printf("NIFReadEntriesExtTest failed.\n");
		goto Exit0;
	}


	/* Close the database. */

	if (error = NSFDbClose (hDbHandle))
	{
		PrintAPIError(error);
		goto Exit1;
	}

Exit0:
	NSFDbClose(hDbHandle);

Exit1:
	NotesTerm();
	
	if (error == NOERROR)
	{
		printf("\nProgram completed successfully.\n");
		return (0);
	}
	else
	{
		printf("\nProgram completed with errors.\n");
		return 1;
	}
}

/************************************************************************

	FUNCTION:
	NIFGetViewRebuildDirTest

	PURPOSE:
	Read the NIFRebuildDirectory path from configuration.

	INPUTS:
	NIL.

	OUTPUTS:
	Returns TRUE if we able to get rebuild directory else returns FALSE.

*************************************************************************/

BOOL LNPUBLIC NIFGetViewRebuildDirTest()
{
	char		szTempDir[MAXPATH] = {0}; /* to store the rebuild directory */

	if (NIFGetViewRebuildDir (szTempDir, sizeof(szTempDir) - 1))
	{
		printf("The rebuild directory is %s.\n", szTempDir);
		return TRUE;
	}
	else
	{
		printf("NIFGetViewRebuildDir failed.\n");
		return FALSE;
	}
}

/************************************************************************

	FUNCTION:	NIFIsTimeVariantViewTest

	PURPOSE:
	To check whether a view is time varying.

	INPUTS:
	Database handle and note id of view.

	OUTPUTS:
	Returns TRUE if view is time-varying else returns FALSE.

*************************************************************************/

BOOL LNPUBLIC NIFIsTimeVariantViewTest(DHANDLE hDb, NOTEID ViewID)
{
	STATUS error = NOERROR;
	HCOLLECTION hCollection = NULLHANDLE;
	
	/* Open the collection. */

	if (error = NIFOpenCollection (hDb, hDb, ViewID, 0, NULLHANDLE, &hCollection, NULL, NULL, NULL, NULL))
	{
		PrintAPIError(error);
		return FALSE;
	}
	
	/* Checks whether the view is time varying. */

	if (NIFIsTimeVariantView (hCollection))
	{
		NIFCloseCollection(hCollection);
		return TRUE;
	}
	else
	{
		NIFCloseCollection(hCollection);
		return FALSE;
	}
}

/************************************************************************

	FUNCTION:	NIFCollectionUpToDateTest

	PURPOSE:
	Checks whether the collection is up to date and
	if it isn't, updates it.

	INPUTS:
	Database handle and note id of view.

	OUTPUTS:
	Returns TRUE if the collection is up to date.

*************************************************************************/

BOOL LNPUBLIC NIFCollectionUpToDateTest(DHANDLE hDB, NOTEID noteid)
{
	STATUS status = NOERROR;

	WORD wUpdateFlags = OPEN_NOUPDATE;
	WORD wUpdateFlags2 = 0;
	HCOLLECTION hCollection = NULLHANDLE;

	printf("\nTesting NIFCollectionUpToDateTest:\n");
	printf("---------------------------------\n");

	status = NIFOpenCollection(hDB, hDB, noteid, wUpdateFlags, NULLHANDLE, &hCollection, NULL, NULL, NULL, NULL);

	if (status)
	{
		PrintAPIError(status);
		return FALSE;
	}

	/* Checks whether the collection is up to date and if it isn't, updates it. */

	if (!NIFCollectionUpToDate (hCollection))
	{
		printf("\nNIFCollection is not up to date; updating now.\n");
		status = NIFUpdateCollection(hCollection);
		
		if (status)
		{
			PrintAPIError(status);
			return FALSE;
		}

		/* After updating, checking whether the collection is up to date. */

		if (NIFCollectionUpToDate (hCollection))
		{
			printf("\nNIFCollection is up to date.\n");
		}
	}
	else
	{
		printf("\nNIFCollection is up to date.\n");
	}
	
	if (status)
	{
		PrintAPIError(status);
		NIFCloseCollection(hCollection);
		return FALSE;
	}

	NIFCloseCollection(hCollection);

	return TRUE;
}

/************************************************************************

	FUNCTION:	NIFIsNoteInViewTest

	PURPOSE:
	Checks whether a note is in the view.

	INPUTS:
	Database handle, note id of view and test note id.

	OUTPUTS:
	NULL.

*************************************************************************/

void LNPUBLIC NIFIsNoteInViewTest(DHANDLE hDB, NOTEID viewnoteid, NOTEID testnoteid)
{
	STATUS error = NOERROR;
	BOOL bResult = FALSE;

	WORD wUpdateFlags = OPEN_NOUPDATE;
	WORD wUpdateFlags2 = 0;
	HCOLLECTION hCollection = NULLHANDLE;

	printf("\nTesting NIFIsNoteInViewTest:\n");
	printf("-----------------------------\n");

	error = NIFOpenCollection(hDB, hDB, viewnoteid, wUpdateFlags, NULLHANDLE, &hCollection, NULL, NULL, NULL, NULL);

	if (error)
	{
		PrintAPIError(error);
		return;
	}

	if (error = NIFIsNoteInView (hCollection, testnoteid, &bResult))
	{
		PrintAPIError(error);
		NIFCloseCollection(hCollection);
		return;
	}
	else
	{
		if (bResult)
			printf("ID  = %d found in collection.\n",testnoteid);
		else
			printf("ID  = %d not found in collection.\n", testnoteid);
	}
	NIFCloseCollection(hCollection);
	return;
}

/************************************************************************

	FUNCTION:	PrintSummary

	PURPOSE:
	Print the summary information about each entry in a view.

	INPUTS:
	Summary to be printed.

	OUTPUTS:
	This function prints the items in the summary for one
	entry in a collection.

*************************************************************************/

STATUS PrintSummary (BYTE *pSummary)
{

/* Local constants. */

#define  MAX_ITEMS          20
#define  MAX_ITEM_LEN       100
#define  DATATYPE_SIZE      sizeof(USHORT)
#define  ITEM_LENGTH_SIZE   sizeof(USHORT)
#define  NUMERIC_SIZE       sizeof(NUMBER)
#define  TIME_SIZE          sizeof(TIMEDATE)


	/* Local variables. */

	BYTE *pSummaryPos = NULL;				/* current position in pSummary */
	ITEM_VALUE_TABLE ItemTable;				/* header at start of pSummary */
	USHORT  ItemCount = 0;					/* number of items in pSummary */
	USHORT  ItemLength[MAX_ITEMS] = {0};	/* length of each item */
	USHORT  DataType = 0;					/* type of pSummary item */
	char szItemText[MAX_ITEM_LEN] = {0};	/* text of a pSummary item */
	NUMBER  NumericItem = 0;				/* a numeric item */
	TIMEDATE   tdTimeItem = {0};			/* a time/date item */
	WORD wTimeStringLen = 0;				/* length of ASCII time/date */
	STATUS  error = NOERROR;				/* return code from API calls */
	USHORT  count = 0;						/* a counter */

	/* Start reading the summary at its beginning. */

	pSummaryPos = pSummary;

	/* Get the header at the beginning of the pSummary buffer. */

	memcpy (&ItemTable, pSummaryPos, sizeof(ItemTable));

	/* Advance the buffer pointer over the header. */

	pSummaryPos += sizeof(ItemTable);

	/* Get the number of items in the summary. */

	ItemCount = ItemTable.Items;
	if (ItemCount > MAX_ITEMS)
	{
		printf ("Summary contains %d items - only printing the first %d items.\n", ItemCount, MAX_ITEMS);
		ItemCount = MAX_ITEMS;
	}

	/* Get the length of each item in the summary. */

	for (count=0; count < ItemCount; count++)
	{
		memcpy (&ItemLength[count], (USHORT *) pSummaryPos, ITEM_LENGTH_SIZE);
		pSummaryPos += ITEM_LENGTH_SIZE;
	}

	/* Start a loop that extracts each item in the summary. */

	for (count=0; count < ItemCount; count++)
	{

		/* If an item has zero length it indicates an "empty" item in the
		summary. This might occur in a lower-level category and stand for a
		higher-level category that has already appeared. Or an empty item might
		be a field that is missing in a response doc. Just print * as a place
		holder and go on to the next item in the pSummary. */

		if (ItemLength[count] == 0)
		{
			printf ("  *  ");
			continue;
		}
		else if (ItemLength[count] >= MAX_ITEM_LEN)
		{

			/* Item is too long for the buffer - just truncate it. */

			ItemLength[count] = MAX_ITEM_LEN - 1;
		}

		/* Get the data type of this item. */

		memcpy (&DataType, (USHORT *) pSummaryPos, DATATYPE_SIZE);
		pSummaryPos += DATATYPE_SIZE;

		/* Extract the item from the summary and put it in readable
		form. The way in which we extract the item depends on its type.
		This program handles TEXT, NUMBER, and TIME. */

		switch (DataType)
		{

			/* Extract a text item from the pSummary. */

			case TYPE_TEXT:
				if ((ItemLength[count] - DATATYPE_SIZE) > 0)
				{
					memcpy (szItemText, pSummaryPos, ItemLength[count] - DATATYPE_SIZE);
					szItemText[ItemLength[count] - DATATYPE_SIZE] = '\0';
				}
				break;

			/* Extract a number item from the pSummary. */

			case TYPE_NUMBER:
				memcpy (&NumericItem, pSummaryPos, NUMERIC_SIZE);
				snprintf (szItemText, NUMERIC_SIZE + 1, "%g", NumericItem);
				break;

			/* Extract a time/date item from the pSummary. */

			case TYPE_TIME: 
				memcpy (&tdTimeItem, pSummaryPos, TIME_SIZE);
				if (error = ConvertTIMEDATEToText(NULL, NULL, &tdTimeItem, szItemText, MAXALPHATIMEDATE, &wTimeStringLen))
					return (ERR(error));
				szItemText[wTimeStringLen] = '\0';
				break;

			/* Checks if the pSummary item is not one of the above data types this program handles. */

			default:
				strncpy (szItemText, "", sizeof(""));
				break;
		}

		/* Print the item (Add spaces so next item is separated). */

		printf ("%s    ", szItemText);

		/* Advance to next item in the pSummary. */

		if ((ItemLength[count] - DATATYPE_SIZE) > 0)
		{
			pSummaryPos += (ItemLength[count] - DATATYPE_SIZE);
		}

		/* End of loop that is extracting each item in the pSummary. */

	}

	/* Print final line feed to end this pSummary display. */

	printf (".\n");

	/* End of function. */

	return (NOERROR);
}

/************************************************************************

	FUNCTION:	NIFReadEntriesExtTest

	PURPOSE:
	Reads the entries in a view and prints the last modified time.

	INPUTS:
	Database handle and note id of view.

	OUTPUTS:
	Returns TRUE if NIFReadEntriesExt was successful and prints the
	last modified time. If there are errors, returns FALSE.

*************************************************************************/

BOOL LNPUBLIC NIFReadEntriesExtTest(DHANDLE hDB, NOTEID ViewID)
{
	HCOLLECTION hCollection = NULLHANDLE;	/* collection handle */
	COLLECTIONPOSITION CollPosition;		/* index into collection */
	DHANDLE     hBuffer = NULLHANDLE;		/* handle to buffer of info */
	BYTE        *pBuffer = NULL;			/* pointer into info buffer */
	BYTE        *pSummary = NULL;			/* pointer into info buffer */
	NOTEID      EntryID;					/* a collection entry id */
	DWORD       dwEntriesFound = 0;			/* number of entries found */
	ITEM_VALUE_TABLE  ItemTable;			/* table in pSummary buffer */
	WORD        wSignalFlag = 0;			/* signal and share warning flags */
	DWORD       dwIndex = 0;				/* a counter */
	STATUS      error = NOERROR;			/* return status from API calls */
	unsigned retNumEntriesSkipped = 0;		/* return skipped entries */
	unsigned short retBufferLength = 0;
	TIMEDATE tdLastModified = {0};			/* returns last modified time */
	TIMEDATE tdRetDiffTime = {0};			/* returns differntial time */
	char szRetTextBuffer[MAXALPHATIMEDATE + 1];
	WORD wRetTextLength = 0;
	
	/* Initializing the szRetTextBuffer. */

	memset(szRetTextBuffer, '\0', MAXALPHATIMEDATE + 1);

	/* Open the collection. */

	error = NIFOpenCollection(hDB, hDB, ViewID, NULL, NULLHANDLE, &hCollection, NULL, NULL, NULL, NULL);
	if (error)
	{
		PrintAPIError(error);
		return FALSE;
	}

	/* Set a COLLECTIONPOSITION to the beginning of the collection. */
	
	CollPosition.Level = 0;
	CollPosition.Tumbler[0] = 0;

	do
	{
		if(error = NIFReadEntriesExt(
				hCollection,					/* handle to this collection */
				&CollPosition,					/* where to start in collection */
				NAVIGATE_NEXT,					/* order to use when skipping */
				1L,								/* number to skip */
				NAVIGATE_NEXT,					/* order to use when reading */
				0xFFFFFFFF,						/* max number to read */
				READ_MASK_NOTEID +				
				READ_MASK_SUMMARYVALUES,		/* info we want to read */
				NULL, NULLHANDLE, MAXDWORD, 0,	/* setting differential time and ID table as NULL */
				&hBuffer,						/* handle to info buffer (return)  */
				&retBufferLength,				/* length of info buffer (return) */
				&retNumEntriesSkipped,			/* entries skipped (return) */
				&dwEntriesFound,					/* entries read (return) */
				&wSignalFlag,
				&tdRetDiffTime,					/* place to get the differential time of NSF file */
				&tdLastModified, NULL))

		{
			NIFCloseCollection (hCollection);
			PrintAPIError (error);
			return FALSE;
		}
		
		if (hBuffer == NULL)
		{
			printf("Empty buffer returned by NIFReadEntriesExt.\n");
		}
		else
		{
			pBuffer = (BYTE *) OSLockObject (hBuffer);
			printf ("\n");
			for (dwIndex = 1; dwIndex <= dwEntriesFound; dwIndex++)
			{
				memcpy (&EntryID, pBuffer, sizeof(EntryID));
				pBuffer += sizeof (EntryID);
				memcpy (&ItemTable, pBuffer, sizeof(ItemTable));      
				pSummary = pBuffer;
				pBuffer += ItemTable.Length;
				if (error = PrintSummary (pSummary))
				{
					OSUnlockObject (hBuffer);
					OSMemFree (hBuffer);
					NIFCloseCollection (hCollection);
					PrintAPIError (error);  
					return FALSE;
				}
			}
			OSUnlockObject (hBuffer);
			OSMemFree (hBuffer);
		}
	}while (wSignalFlag & SIGNAL_MORE_TO_DO);
	
	if (error = NIFCloseCollection (hCollection))
	{
		PrintAPIError (error);  
		return FALSE;
	}

	/* Converting TIMEDATE to TEXT. */

	if (error = ConvertTIMEDATEToText (NULL, NULL, &tdRetDiffTime, szRetTextBuffer, MAXALPHATIMEDATE+1, &wRetTextLength))
	{
		PrintAPIError (error);
		return FALSE;
	}
	printf("Last update to NSF file was on = %s.\n", szRetTextBuffer);
	return TRUE;
}

/************************************************************************

	FUNCTION:	NSFNoteCreateTest

	PURPOSE:
	Creates a note id under the Main View of Database.

	INPUTS:
	Database handle.

	OUTPUTS:
	Returns TRUE if the note ID is created, else returns FALSE.

*************************************************************************/

BOOL LNPUBLIC NSFNoteCreateTest(DHANDLE hDB)
{
	NOTEHANDLE  hNote;
	STATUS error = NOERROR;
	
	/* Creates a new data note. */

	if (error = NSFNoteCreate (hDB, &hNote))
	{
		PrintAPIError (error);  
		return FALSE;
	}

	/* Writes a field named FORM to the note. */

	if (error = NSFItemSetText (hNote, "FORM", "Main View", MAXWORD))
	{
		NSFNoteClose (hNote);
		PrintAPIError (error);
		return FALSE;
	}

	/* Writes a text field named PLAIN_TEXT to the note. */

	if (error = NSFItemSetText ( hNote, "PLAIN_TEXT", "Notes document created to test NIFReadEntriesExt", MAXWORD))
	{
		NSFNoteClose (hNote);
		PrintAPIError (error);
		return FALSE;
	}

	/* Writes a field named NUMBER to the note. */

	if (error = NSFItemSetText ( hNote, "NUMBER", "15", 2))
	{
		NSFNoteClose (hNote);
		PrintAPIError (error);
		return FALSE;
	}
	
	/* Writes a field named TIME_DATE to the note. */

	if (error = NSFItemSetText ( hNote, "TIME_DATE", "15-12-1997", 10))
	{
		NSFNoteClose (hNote);
		PrintAPIError (error);
		return FALSE;
	}

	if (error = NSFNoteUpdate (hNote, 0))
	{
		NSFNoteClose (hNote);
		PrintAPIError (error);
		return FALSE;
	}
	
	if (error = NSFNoteClose (hNote))
	{
		PrintAPIError (error);
		return FALSE;
	}
	printf("Updating and closing note.\n");
	return TRUE;
}

/* This function prints the error message associated with an error code. */

void PrintAPIError (STATUS api_error)

{
	STATUS  string_id = ERR(api_error);
	char    szErrorText[256] = {0};
	WORD    wTextLen = 0;

	/* Get the message for this error code from the resource string table. */

	wTextLen = OSLoadString (NULLHANDLE,
		string_id,
		szErrorText,
		sizeof(szErrorText));

	/* Print error message. */

	fprintf (stderr, "\n%s.\n", szErrorText);

}

/************************************************************************

	FUNCTION:	ProcessArgs

	PURPOSE:
	To parse command line arguments.

	INPUTS:
	argc, argv - directly from the command line.

	OUTPUTS:
	szDBFileName, szTimeVariantView, szViewName - data from 
	the command line or the user input when prompted.

*************************************************************************/
void  LNPUBLIC  ProcessArgs (int argNumber, char *szArgVector[], char *szDBFileName, char *szTimeVariantView, char *szViewName)
{
	if (argNumber != 4)
	{
		printf("Enter the name of Database: ");
		fflush(stdout);
		fgets(szDBFileName, STRING_LENGTH-1, stdin);
		szDBFileName[strlen(szDBFileName) - 1] = '\0';

		printf("Enter the name of Time Variant View: ");
		fflush(stdout);
		fgets(szTimeVariantView, STRING_LENGTH-1, stdin);
		szTimeVariantView[strlen(szTimeVariantView) - 1] = '\0';

		printf("Enter the name of view that is not time varying: ");
		fflush(stdout);
		fgets(szViewName, STRING_LENGTH-1, stdin);
		szViewName[strlen(szViewName) - 1] = '\0';
	}
	else
	{
		strncpy(szDBFileName, szArgVector[1], STRING_LENGTH-1);
		strncpy(szTimeVariantView, szArgVector[2], STRING_LENGTH-1);
		strncpy(szViewName, szArgVector[3], STRING_LENGTH-1);
	} /* end if */
} /* ProcessArgs */
