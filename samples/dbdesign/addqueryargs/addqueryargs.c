/****************************************************************************

	PROGRAM:	addqueryargs

	FILE:		addqueryargs.c
	
	PURPOSE:
	This program demonstrates how to use the 
	NSFQueryDBAddArgs API.

	SYNTAX:
	addqueryargs

	Description:
	This program builds a lists of values to input into a DQL query.
	
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

/* Function prototypes for local functions. */

BOOL LNPUBLIC NSFQueryTest ();
void PrintAPIError (STATUS);

/************************************************************************

	FUNCTION:   main API subroutine

*************************************************************************/

int main(int argc, char *argv[])
{

	/* Local data declarations. */

	STATUS   error = NOERROR;                    /* return status from API calls */

	/* Initialize Domino and Notes. */

	if (error = NotesInitExtended (argc, argv))
	{
		PrintAPIError (error);
		return (1);
	}

	printf("Testing NSFQueryDBAddArgs:\n");
	printf("--------------------------\n");

	if (NSFQueryTest ())
	{
		printf("NSFQueryDBAddArgs was successful.\n");
	}
	else
	{
		printf("NSFQueryDBAddArgs failed.\n");
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
	To build the argument list and print the arguments.

	INPUTS:
	No inputs.

	OUTPUTS:
	Returns TRUE if the input text is converted to 
	an argument list and printed, else returns FALSE.

*************************************************************************/

BOOL LNPUBLIC NSFQueryTest ()
{
	int start = 0;
	DWORD dwIndex = 0;		/* Position of entries in argList */
	QUEP_ARGVAL argValue = {0};
	QUEP_ARGLIST *argList = NULL;
	STATUS error = NOERROR;
	MEMHANDLE hTestArgList = NULLMEMHANDLE;	/* To store argument list */
    
	printf("Building argument list:\n");
	for (start = 0; start < 3; start++)
	{
		if (start == 0)
		{
			argValue.type = TYPE_TEXT;
			strncpy(argValue.Value, "\'This is another line of simple text.\'", sizeof(argValue.Value));
		}
		else if (start == 1)
		{
			argValue.type = TYPE_NUMBER;
			strncpy(argValue.Value, "836", sizeof(argValue.Value));
		}
		else
		{
			argValue.type = TYPE_TIME;
			strncpy(argValue.Value, "@dt('2019-01-01T16:42:00')", sizeof(argValue.Value));
		}
		argValue.length = strlen(argValue.Value);
		argValue.bBinaryForm = FALSE;

		/* Building argument list. */

		if (error = NSFQueryDBAddArgs(&argValue, &hTestArgList))
		{
			goto errout;
		}

		/* Printing the argument list. */

		argList = (QUEP_ARGLIST *)OSMemoryLock(hTestArgList);
		if (argList != NULL)
		{
			for (dwIndex=0; dwIndex<argList->numargs; dwIndex++)
			{
				printf("Value: %s\t",argList->QArgs[dwIndex].Value);
				printf("Length: %d\n", argList->QArgs[dwIndex].length);
			}
		}
		OSMemoryUnlock(hTestArgList);
		argList = NULL;
		if (hTestArgList != NULLMEMHANDLE)
		{
			OSMemoryFree(hTestArgList);
			hTestArgList = NULLMEMHANDLE;
		}
	}
	return TRUE;

errout:
	PrintAPIError (error);
	return FALSE;
}

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
