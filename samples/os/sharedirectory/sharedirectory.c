/*************************************************************************

PROGRAM:	sharedirectory

FILE:		sharedirectory.c

PURPOSE:	Prints the shared data directory path of the Notes client using OSGetSharedDataDirectory.

SYNTAX:		sharedirectory

*************************************************************************/
/* C include files. */

#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files. */

#include "global.h"
#include "osfile.h"
#include "osmisc.h"
#include "miscerr.h"
#include "oserr.h"

void PrintAPIError (STATUS);

/* Program declaration. */
int main(int argc, char *argv[])
{
	
	/* Local data declarations. */
	
	char szShareDirectory[MAXPATH];
	STATUS error = NOERROR;
	memset(szShareDirectory, MAXPATH, '\0');

	error = NotesInitExtended(argc, argv);

	if (error != NOERROR)
	{
		PrintAPIError(error);
		return (1);
	}

	/* This api is applicable only to the client. 
	Gets share data directory of the client. */

	if (OSGetSharedDataDirectory (szShareDirectory) == 0)
	{
		printf("OSGetSharedDataDirectory failed.\n");
	}
	else
	{
		printf("The shared data directory of the multi-user Notes client is \"%s\".\n", szShareDirectory);
		printf("Program completed successfully.\n");
	}
	NotesTerm();
	return (0);
}

/*************************************************************************

    FUNCTION:  PrintAPIError

    PURPOSE:   This function prints the error message 
               associated with an error code.

**************************************************************************/

void PrintAPIError (STATUS api_error)

{
	STATUS  string_id = ERR(api_error);
	char    szErrorText[256] = {0};
	WORD    wtext_len = 0;

	/* Get the message for this error code from the resource string table. */

	wtext_len = OSLoadString (NULLHANDLE,
		string_id,
		szErrorText,
		sizeof(szErrorText));

	/* Print error message. */
	fprintf (stderr, "\n%s.\n", szErrorText);

}
