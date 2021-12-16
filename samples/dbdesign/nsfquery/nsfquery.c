/****************************************************************************

	PROGRAM:	nsfquery

	FILE:		nsfquery.c
	
	PURPOSE:
	This program demonstrates how to use the 
	NSFQueryDBExt2 API.

	SYNTAX:
	nsfquery  <db filename>
		or
	nsfquery
	
	Example: nsfquery simple.nsf

	Description:
	This program writes the queries to a file, extracts the query from 
	the file to query the database and then deletes the file.
	
****************************************************************************/

/* C include files. */
#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files. */

#include <global.h>
#include <nsfdb.h>
#include <osmem.h>
#include <dbmiscods.h>
#include <misc.h>
#include <dbmisc.h>
#include <miscerr.h>
#include <oserr.h>
#include <osmisc.h>

#define  STRING_LENGTH  256
#define  OUTFILE  "query.tmp"

/* Function prototypes for local functions. */

void  LNPUBLIC  ProcessArgs (int argc, char *argv[], char *szDBFileName);
void PrintAPIError (STATUS);
BOOL LNPUBLIC NSFQueryTest (DHANDLE hDB);

/************************************************************************

	FUNCTION:   main API subroutine

*************************************************************************/

int main(int argc, char *argv[])
{

	/* Local data declarations. */
	FILE *outFile = NULL;
	char *szDBFileName = NULL;				/* name of the database */
	char szDBBuf[STRING_LENGTH] = {0};
	DBHANDLE hDB = NULLHANDLE;				/* handle of the database */
	STATUS error = NOERROR;					/* return status from API calls */

	szDBFileName = szDBBuf;
	
	ProcessArgs (argc, argv, szDBFileName);

	/* Initialize Domino and Notes. */

	if (error = NotesInitExtended (argc, argv))
	{
		PrintAPIError (error);
		return (1);
	}

	outFile = fopen (OUTFILE, "w");
	if (!outFile)
	{
		printf("Error opening the file %s.", OUTFILE);
		return (1);
	}
	else
	{
		fprintf(outFile, "PLAIN_TEXT = \'This is another line of simple text.\'\n");
		fprintf(outFile, "NUMBER = 836\n");
		fprintf(outFile, "TIME_DATE = @dt('2019-01-01T16:42:00')\n");
		fclose(outFile);
	}

	/* Open the database. */

	if (error = NSFDbOpen (szDBFileName, &hDB))
	{
		PrintAPIError (error);  
		NotesTerm();
		return (1);
	}

	printf("Testing NSFQueryDBExt2:\n");
	printf("-----------------------\n");

	if (NSFQueryTest (hDB))
	{
		printf("NSFQueryDBExt2 was successful.\n");
	}
	else
	{
		printf("NSFQueryDBExt2 failed.\n");
	}

	/* Close the database. */

	if (error = NSFDbClose (hDB))
	{
		PrintAPIError (error);  
		NotesTerm();
		return (1);
	}

	/* End of subroutine. */
	NotesTerm ();
	if (error == NOERROR)
	{
		printf("\nProgram completed successfully.\n");
		return (0);
	}
}

/************************************************************************

	FUNCTION:	NSFQueryTest

	PURPOSE:
	Extracts query from the file and runs it on the database.

	INPUTS:
	Database handle.

	OUTPUTS:
	Returns TRUE if the database is queried, else returns FALSE.

*************************************************************************/

BOOL LNPUBLIC NSFQueryTest (DHANDLE hDB)
{
	FILE *inFile = NULL;
	STATUS error = NOERROR;
	char szQuery[MAXPATH] = {0};			/* Place to store query */
	DHANDLE hRetResults = NULLHANDLE;		/* To store results */
	MEMHANDLE hRetError = NULLMEMHANDLE;	/* To store the error */
	MEMHANDLE hRetExplain = NULLMEMHANDLE;	/* To store the explanation of query */
	WORD wTranslatedKeyLen = 0;
	char szTranslatedKey[MAXPATH] = {0};	/* To store LMBCS query */
    
	inFile = fopen (OUTFILE, "r");
	if (!inFile)
	{
		printf("Error opening the file %s.", OUTFILE);
		return FALSE;
	}
	else
	{
		while (fgets (szQuery, MAXPATH, inFile))
		{
			/* Removes trailing new line character from szQuery */

			szQuery[strlen(szQuery) - 1] = '\0';

			/* Converts query to LMBCS. */

			wTranslatedKeyLen = OSTranslate (OS_TRANSLATE_NATIVE_TO_LMBCS, szQuery, (WORD) strlen (szQuery), szTranslatedKey, sizeof(szTranslatedKey));

			/* To run DQL queries on the database. */

			if (error = NSFQueryDBExt2 (hDB, szTranslatedKey, wTranslatedKeyLen, QUEP_EXPLAIN, 0, 0, 0, &hRetResults, &hRetError, &hRetExplain, NULL))
			{
				goto errout;
			}
			if (hRetExplain != NULLMEMHANDLE)
			{
				char *explain = (char *)OSMemoryLock(hRetExplain);
				printf("%s\n", explain);
				OSMemoryUnlock(hRetExplain);
				OSMemoryFree(hRetExplain);
				hRetExplain = NULLMEMHANDLE;
			}
			if (hRetResults != NULLHANDLE)
			{
				OSMemFree(hRetResults);
				hRetResults = NULLHANDLE;
			}
			if (hRetError != NULLMEMHANDLE)
			{
				OSMemoryFree(hRetError);
				hRetError = NULLMEMHANDLE;
			}
			memset(szQuery, '\0', sizeof(szQuery));
			memset(szTranslatedKey, '\0', sizeof(szTranslatedKey));
		}
		fclose(inFile); 
		remove(OUTFILE);
	}
	return TRUE;

errout:
	PrintAPIError (error);
	return FALSE;
}

/************************************************************************

	FUNCTION:	ProcessArgs

	PURPOSE:
	To parse command line arguments.

	INPUTS:
	argc, argv - directly from the command line.
   
	OUTPUTS:
	szDBFileName - data from the command line or 
	the user input when prompted.

*************************************************************************/

void  LNPUBLIC  ProcessArgs (int argc, char *argv[], char *szDBFileName)
{ 
	if (argc != 2)  
	{
		printf("Enter name of database: ");      
		fflush(stdout);
		fgets(szDBFileName, STRING_LENGTH-1, stdin);
	}
	else
	{
		strncpy(szDBFileName, argv[1], STRING_LENGTH-1);
	} /* end if */
} /* ProcessArgs */

/*************************************************************************

    FUNCTION:   PrintAPIError

    PURPOSE:
	This function prints the error message associated
	with an error code.

**************************************************************************/

void PrintAPIError (STATUS api_error)
{
	STATUS  string_id = ERR(api_error);
	char    szErrorText[256] = {0};
	WORD    wTextLen = 0;

	/* Get the message for the error code from the resource string table. */

	wTextLen = OSLoadString (NULLHANDLE, string_id, szErrorText,sizeof(szErrorText));

	/* Print it. */

	fprintf (stderr, "\n%s.\n", szErrorText);
}
