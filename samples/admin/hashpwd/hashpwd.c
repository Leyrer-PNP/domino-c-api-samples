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

    PROGRAM:   hashpass

    FILE:      hashpass.c

	PURPOSE:   This program will use various methods to create digest for the ID file which 
			   run this program, and then verify the passowrd.

    SYNTAX:    hashpass


*************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */
#include <stdio.h>
#include <string.h>
/* Notes API include files */
#if defined (OS390)
#include <lapicinc.h>
#endif

#include <global.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <ostime.h>
#include <osmisc.h>
#include "misc.h"
#include "secerr.h"
#include "bsafe.h"

/* Local function prototypes */
void PrintAPIError (STATUS);

/* Notes API subroutine */
int main (int argc, char *argv[])
{
	WORD	Digest2Len = 0;
	BYTE	Digest2 [52];
	WORD	Digest3Len = 0;
	BYTE	Digest3 [52];
	STATUS	Verify = NOERROR;
	STATUS	error;

	if (error = NotesInitExtended (argc, argv))
	 {
        printf("\n Unable to initialize Notes.\n");
        return (1);
	 }

	if(error = SECHashPassword (strlen ("viking"), (BYTE*) "viking",
		52, &Digest2Len, Digest2, 0, NULL))
	{
		PrintAPIError (error);
	}

	Verify = SECVerifyPassword(strlen ("viking"), (BYTE*)"viking",
										Digest2Len, Digest2, 0, NULL);

	if (Verify != NOERROR)
		printf("Verify good password fail.\n");
	else
		printf("Verify good password pass.\n");
						   	 
	Verify = SECVerifyPassword(strlen ("viking2"), (BYTE*)"viking2", Digest2Len, Digest2, 0, NULL);
	
	if (Verify != ERR_SECURE_BADPASSWORD)
		printf("Verify bad password fail.\n");
	else
		printf("Verify bad password pass.\n");

	if(error = SECHashPassword3(strlen ("viking"), (BYTE*)"viking",
					SEC_pwddigest_V1, 0, NULL,
					12345, NULL, 0,
					52, &Digest3Len, Digest3, 0, NULL))
	{
		PrintAPIError (error);
	}

	Verify = SECVerifyPassword(strlen ("viking"), (BYTE*)"viking",
								Digest3Len, Digest3, 0, NULL);

	if (Verify != NOERROR)
		printf("Verify good password fail.\n");
	else
		printf("Verify good password pass.\n");
						   	 

	if(error = SECHashPassword3(strlen ("viking"), (BYTE*)"viking",
					SEC_pwddigest_V2, 0, NULL,
					12345, NULL, 0,
					52, &Digest3Len, Digest3, 0, NULL))
	{
		PrintAPIError (error);
	}

	Verify = SECVerifyPassword(strlen ("viking"), (BYTE*)"viking",
								Digest3Len, Digest3, 0, NULL);

	if (Verify != NOERROR)
		printf("Verify good password fail.\n");
	else
		printf("Verify good password pass.\n");

	if(error = SECHashPassword3(strlen ("viking"), (BYTE*)"viking",
					SEC_pwddigest_V3, SEC_ai_HMAC_SHA1, NULL,
					12345, NULL, 0,
					52, &Digest3Len, Digest3, 0, NULL))
	{
		PrintAPIError (error);
	}

	Verify = SECVerifyPassword(strlen ("viking"), (BYTE*)"viking", Digest3Len, Digest3, 0, NULL);

	if (Verify != NOERROR)
		printf("Verify good password fail.\n");
	else
		printf("Verify good password pass.\n");

	Verify = SECVerifyPassword(strlen ("viking2"), (BYTE*)"viking2", Digest3Len, Digest3, 0, NULL);
	
	if (Verify != ERR_SECURE_BADPASSWORD)
		printf("Verify bad password fail.\n");
	else
		printf("Verify bad password pass.\n");

	NotesTerm();

	return	0;
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

