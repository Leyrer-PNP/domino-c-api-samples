/*************************************************************************

PROGRAM:    getmultnoteinfo

FILE:       getmultnoteinfo.c

PURPOSE:    Shows the basic usage of NSFDbGetMultNoteInfo & 
NSFDbGetMultNoteInfoByUNID. The program prints NoteIDs & 
UNIDs to the screen.

SYNTAX:     getmultnoteinfo [server name - optional] <database filename>

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
#include "ods.h"
#include "nsfsearc.h"
#include "idtable.h"
#include "nsfnote.h"
#include "osmem.h"

#if defined(OS390)
#include "lapicinc.h"
#endif
#include "lapiplat.h"

/* NOTE: This code MUST be the LAST file included so that ascii versions of the system APIs are used     */
#if defined(OS390) && (__STRING_CODE_SET__==ISO8859-1 /* If os390 ascii compile                          */)     
#include <_Ascii_a.h>
#endif

#define MaxNoteNum 30

/* Local function prototypes */

void PrintAPIError (STATUS);
STATUS LNPUBLIC AddIDUnique (void far *, SEARCH_MATCH far *, ITEM_TABLE far *);


WORD				NumIDs = 0;
UNID				unid[MaxNoteNum];

/* Program declaration */
int main(int argc, char *argv[])
{
	/* Local data declarations */

	char		pname[MAXPATH] = "";         /* buffer to store the input path to database */
	char		*path_name;                  /* pathname of database */
	char		*server_name;                /* server name where database lives*/
	char		*db_name;                    /* name of database */
	int			ArgNum = 0;
	int			i = 0;	

	DBHANDLE		hDB;
	STATUS			error = NOERROR;

	char			*UNIDTable,			*NoteIDTable;
	NOTEID			ID;

	DHANDLE			hNoteIDTable,		hUNIDTable;
	DHANDLE			hIDTable;
	DHANDLE			hInfoTable,			hInfoTableByUNID;
	DWORD			InfoTableLength,	InfoTableLengthByUNID;
	char			*InfoTable,			*InfoTableByUNID;
	NOTEID			id[MaxNoteNum],		idByUNID[MaxNoteNum];
	OID				oid[MaxNoteNum],	oidByUNID[MaxNoteNum];
	BOOL			First;
	NOTEHANDLE              noteHandle;
	BOOL                    bFieldFound;
	WORD                    wFieldLen;
	char                    szFieldText[500] = {0};
	UNID                    *pUnid = NULL;

#if defined(OS390)
	char XLATE_path_name[MAXPATH] = "";     /* path name translation buffer */
	char XLATE_title[NSF_INFO_SIZE] = "";   /* database title translation buffer */
#endif

	/* Initialize pointers to point to "" */
	path_name = pname;
	server_name = pname;
	db_name = pname;

	ArgNum = argc;

	/* Get the pathname of the database from the command line. */
#if defined(OS390) && (__STRING_CODE_SET__==ISO8859-1 /* If os390 ascii compile                          */)     
	__argvtoascii_a(argc,argv);                      /* Convert args from EBCDIC (Native z/OS) to ASCII */
#endif
	if((ArgNum < 2) || (ArgNum >3))
	{
		printf( "\nUsage:  %s  [server name - optional] <database filename>\n", argv[0] );
		return (0);
	}

	db_name = argv[ArgNum - 1];
	path_name = db_name;


	if (ArgNum == 3)
	{
		server_name = argv[1];
	}

	error = NotesInitExtended (argc, argv);

	if (error)
	{
		fprintf (stderr, "\nError initializing Notes.\n");
		return (1);
	}

	if (strcmp (server_name, ""))
	{
		if (error = OSPathNetConstruct( NULL, server_name, db_name, pname))
		{
			PrintAPIError (error);
			NotesTerm();
			return (1);
		}
		path_name = pname;
	}

	/* Open the database. */

	if (error = NSFDbOpen (path_name, &hDB))
	{
		PrintAPIError (error);
		NotesTerm();
		return (1);
	}

	if (error = IDCreateTable(sizeof(NOTEID), &hIDTable))
	{
		PrintAPIError (error);
		NSFDbClose (hDB);
		NotesTerm();
		return (1);
	}

	if (error = NSFSearch (
		hDB,            /* database handle */
		NULLHANDLE,     /* selection formula (select all notes) */
		NULL,           /* title of view in selection formula */
		0,              /* search flags */
		NOTE_CLASS_DOCUMENT,/* note class to find */
		NULL,           /* starting date (unused) */
		AddIDUnique,    /* call for each note found */
		&hIDTable,  /* argument to AddIDUnique */
		NULL))          /* returned ending date (unused) */
	{
		PrintAPIError (error);
		IDDestroyTable(hIDTable);
		NSFDbClose (hDB);
		NotesTerm();
		return (1);
	}


	if (error = OSMemAlloc(0, NumIDs * ODSLength(_NOTEID), &hNoteIDTable))
	{
		PrintAPIError (error);
		IDDestroyTable(hIDTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	NoteIDTable = OSLock(char, hNoteIDTable);

	First = TRUE;
	i = 0;
	while ( i < NumIDs && IDScan(hIDTable, First, &ID))
	{
		ODSWriteMemory(&NoteIDTable, _NOTEID, &ID, 1);
		i++;
		First = FALSE;
	}

	OSUnlock(hNoteIDTable);
	IDDestroyTable(hIDTable);

	/*************************************************************************
	**	Call NSFGetMultNoteInfo routine.
	*************************************************************************/
	if (error = NSFDbGetMultNoteInfo(hDB, NumIDs, fINFO_OID, hNoteIDTable, &InfoTableLength, &hInfoTable))
		PrintAPIError (error);
	OSMemFree(hNoteIDTable);
	if (error)
	{
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	/*************************************************************************
	**	Check the number of note infos returned.
	*************************************************************************/
	if ( NumIDs * (sizeof(NOTEID) + sizeof(OID)) != InfoTableLength)
	{
		printf("Error in NSFDbGetMultNoteInfo: Number of note infos returned does not match.\n");
		OSMemFree(hInfoTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	InfoTable =(char *) OSLockObject(hInfoTable);
	for (i=0; i<NumIDs; i++)
	{
		ODSReadMemory(&InfoTable, _NOTE_ID, id+i, 1);
		ODSReadMemory(&InfoTable, _OID, oid+i, 1);
	}


	/*************************************************************************
	**	Construct the UNID table.
	*************************************************************************/
	if (error = OSMemAlloc(0, NumIDs * ODSLength(_UNID), &hUNIDTable))
	{
		PrintAPIError (error);
		OSUnlockObject(hInfoTable);
		OSMemFree(hInfoTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	UNIDTable = OSLock(char, hUNIDTable);

	ODSWriteMemory(&UNIDTable, _UNID, unid , NumIDs); /* unid is a UNID array */

	OSUnlock(hUNIDTable);

	/*************************************************************************
	**	Call NSFGetMultNoteInfoByUNID routine.
	*************************************************************************/
	if (error = NSFDbGetMultNoteInfoByUNID(hDB, NumIDs, fINFO_NOTEID|fINFO_OID, hUNIDTable, &InfoTableLengthByUNID, &hInfoTableByUNID))
	{
		PrintAPIError (error);
	}
	OSMemFree(hUNIDTable);
	if (error)
	{
		OSUnlockObject(hInfoTable);
		OSMemFree(hInfoTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	/*************************************************************************
	**	Call NSFNoteOpenByUNIDExtended routine.
	*************************************************************************/
	pUnid = unid;
	if (error = NSFNoteOpenByUNIDExtended(hDB, pUnid, 0, &noteHandle))
	{
		PrintAPIError (error);
		OSUnlockObject(hInfoTable);
		OSMemFree(hInfoTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	/* Look for the TIME_DATE field within this note. */

	bFieldFound = NSFItemIsPresent (
		noteHandle,
                "TIME_DATE",
                (WORD) strlen ("TIME_DATE"));

    	/* If the TIME_DATE field is there, get the contents of the field as an
     	ASCII string and print it out. */

	if (bFieldFound)
	{
		wFieldLen = NSFItemConvertToText (
			noteHandle,
                	"TIME_DATE",
			szFieldText,
			(WORD) sizeof (szFieldText),
                	';'); /* multi-value separator */

		printf ("TIME_DATE field is: %s\n", szFieldText);
    	}

   	/* If the TIME_DATE field is not there, print a message. */

    	else
        	printf ("TIME_DATE field not found.\n");

	/* Close the note. */

    	if (error = NSFNoteClose(noteHandle))
	{
		PrintAPIError (error);
		OSUnlockObject(hInfoTable);
		OSMemFree(hInfoTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	/*************************************************************************
	**	Check the number of IDs returned.
	*************************************************************************/
	if (( NumIDs * (sizeof(NOTEID) + sizeof(OID) )) != InfoTableLengthByUNID)
	{
		printf("Error in NSFDbGetMultNoteInfoByUNID: Number of UNIDs returned does not match.\n");
		OSMemFree(hInfoTableByUNID);
		OSUnlockObject(hInfoTable);
		OSMemFree(hInfoTable);
		NSFDbClose (hDB);
		NotesTerm();
		return(1);
	}

	InfoTableByUNID =(char *) OSLockObject(hInfoTableByUNID);
	for (i=0; i<NumIDs; i++)
	{
		ODSReadMemory(&InfoTableByUNID, _NOTE_ID, idByUNID + i, 1);
		ODSReadMemory(&InfoTableByUNID, _OID, oidByUNID + i, 1);
	}


	for (i=0; i<NumIDs; i++)
	{
		printf("id[%d] got by NoteID is %X\n", i ,id[i]);
		printf("idByUNID[%d] got by UNID is %X\n", i ,idByUNID[i]);

		printf("oid[%d] got by NoteID has a UNID %08X:%08X-%08X:%08X\n", i ,oid[i].File.Innards[1], oid[i].File.Innards[0], oid[i].Note.Innards[1], oid[i].Note.Innards[0]);
		printf("oidByUNID[%d] got by UNID has a UNID %08X:%08X-%08X:%08X\n", i, oidByUNID[i].File.Innards[1], oidByUNID[i].File.Innards[0], oidByUNID[i].Note.Innards[1], oidByUNID[i].Note.Innards[0]);
	}

	OSUnlockObject(hInfoTable);
	OSMemFree(hInfoTable);
	OSUnlockObject(hInfoTableByUNID);
	OSMemFree(hInfoTableByUNID);

	if (error = NSFDbClose (hDB))
	{
		PrintAPIError (error);
		NotesTerm();
		return (1);
	}

	if (error == NOERROR)
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
#if defined(OS390) && (__STRING_CODE_SET__!=ISO8859-1 /* ebcdic compile */)
	char    NATIVE_error_text[200];
#endif /* OS390, ebcdic compile */

	/* Get the message for this HCL C API for Notes/Domino error code
	from the resource string table. */

	text_len = OSLoadString (NULLHANDLE,
		string_id,
		error_text,
		sizeof(error_text));

	/* Print it. */

#if defined(OS390) && (__STRING_CODE_SET__!=ISO8859-1 /* ebcdic compile */)
	OSTranslate(OS_TRANSLATE_LMBCS_TO_NATIVE, error_text, MAXWORD, NATIVE_error_text, sizeof(NATIVE_error_text));
	fprintf (stderr, "\n%s\n", NATIVE_error_text);
#else
	fprintf (stderr, "\n%s\n", error_text);
#endif /* OS390, ebcdic compile */

}

/************************************************************************

FUNCTION:   AddIDUnique                                             

*************************************************************************/

STATUS LNPUBLIC AddIDUnique    
(void far * phNoteIDTable,
 SEARCH_MATCH far *pSearchMatch,
 ITEM_TABLE far *summary_info)
{
	SEARCH_MATCH SearchMatch;
	DHANDLE    hNoteIDTable;
	STATUS       error = NOERROR;
	BOOL         flagOK;

	if( NumIDs >= MaxNoteNum )
	{
		return (ERR(error));
	}

	memcpy( (char*)&SearchMatch, (char*)pSearchMatch, sizeof(SEARCH_MATCH) );

	if (!(SearchMatch.SERetFlags & SE_FMATCH))
		return (NOERROR);

	hNoteIDTable = *((DHANDLE far *)phNoteIDTable);

	if (error = IDInsert(hNoteIDTable, SearchMatch.ID.NoteID, &flagOK))
	{
		printf ("Error: unable to insert note ID into table.\n");
		return (ERR(error));
	}

	if (flagOK == TRUE)
	{
		printf ("\tInserted note %1X into table.\n", SearchMatch.ID.NoteID);
		unid[NumIDs].File = SearchMatch.OriginatorID.File;
		unid[NumIDs].Note = SearchMatch.OriginatorID.Note;
		printf("\tSearchMatch has a UNID %08X:%08X-%08X:%08X\n", unid[NumIDs].File.Innards[1], unid[NumIDs].File.Innards[0], unid[NumIDs].Note.Innards[1], unid[NumIDs].Note.Innards[0] );
		++ NumIDs;
	}
	else
	{
		printf ("\tNote %lX is already in table.\n", SearchMatch.ID.NoteID);
	}   

	return (ERR(error));
}
