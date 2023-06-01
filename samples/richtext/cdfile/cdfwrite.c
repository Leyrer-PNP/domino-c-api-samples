/**************************************************************************
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

    PROGRAM:    cdfwrite
    
    FILE:       cdfwrite.c 

    SYNTAX:     cdfwrite 

    PURPOSE:    Creates a file containing Domino and Notes rich text data.

    DESCRIPTION:
        This program creates a CD file, cd_recs.inp, which contains 
        the specified text.  A CD file is a file containg Domino and Notes 
        rich text. You may use this CD file to test the API function 
        CompoundTextAssimilateFile(), which takes a CD file as input.

        A CD file starts with the datatype word TYPE_COMPOSITE. Following
        the datatype word is a series of Compound Document (CD) records.
        All data in the file is in Domino and Notes Canonical Format. 

****************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */

#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files */

#include <global.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <editods.h>
#include <editdflt.h>
#include <stdnames.h>
#include <ostime.h>
#include <easycd.h>
#include <fontid.h>
#include <osmem.h>
#include <nif.h>

#include <osmisc.h>

/* local include files */
#include "cdfile.h"

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                             char *szText); 

void PrintAPIError (STATUS);

/**************************************************************************

    FUNCTION:   main

***************************************************************************/

int main(int argc, char *argv[])
{
    char           *szCDFileName;
    char            szText[128];
    STATUS          nErr = NOERROR;  /* return code from API calls */
    DHANDLE           hCompound;       /* handle to CompoundText context */
    COMPOUNDSTYLE   Style;
    DWORD           dwStyleID;
    DHANDLE           hCDBuffer;       /* handle to buffer of CD records */
    DWORD           dwBufferSize;
    SIZE_T          sBufferSize;
    char            szTempFileName[MAXPATH+1];  /* created by Domino and Notes */
    char           *pData;          /* pointer to buffer of CD records */
    FILE           *pCDFile;        /* output file stream */
    SIZE_T          sRetVal;        /* return value from fwrite() */
    char            eString[] = "";
    char            NewLine[] = "\n"; /* newline delimiter */

    szCDFileName = CD_FILENAME;

	if (nErr = NotesInitExtended (argc, argv))
	{
		printf("\n Unable to initialize Notes.\n");
		return (1);
	}


    /* process command line arguments or prompt user for arguments */
    ProcessArgs (argc, argv, szText);
    
    /* Create a "stand-alone" Compound Text context */

    nErr = CompoundTextCreate (
             NULLHANDLE,        /* no note handle */
             NULL,              /* no item name */
             &hCompound);       /* returned handle to CompoundText context */

    if (nErr != NOERROR)
    {
        printf( "Error: unable to create CompoundText context.\n" );
	    PrintAPIError (nErr);  
	    NotesTerm();
		return (1);
    }

    /* Add text from szString to the CompoundText context */

    CompoundTextInitStyle (&Style); /* initializes Style to the defaults */

    nErr = CompoundTextDefineStyle (
                hCompound,      /* handle to CompoundText context */
                eString,             /* style name - none */
                &Style,
                &dwStyleID);    /* style id */
 
   if (nErr != NOERROR)
   {
       printf( "Error: unable to define CompoundText style no. 1.\n" );
	   PrintAPIError (nErr);  
	   NotesTerm();
	   return (1);
   }

   nErr = CompoundTextAddTextExt (
             hCompound,               /* handle to CompoundText context */
             dwStyleID,               /* style ID */
             DEFAULT_FONT_ID,         /* font ID */
             szText,                  /* text to add */
             (DWORD) strlen (szText), /* length of text */
             NewLine,                    /* newline delimiter */
             COMP_PRESERVE_LINES,     /* preserve line breaks */
             NULLHANDLE);             /* handle of CLS translation table */

    if (nErr != NOERROR)
    {
        printf( "Error: unable to Add text to CompoundText context .\n" );
        CompoundTextDiscard (hCompound);
	    PrintAPIError (nErr);  
	    NotesTerm();
		return (1);
    }

   /* Close the CompoundText context in stand-alone mode. This returns the
      sequence of CD records in a buffer, or, if more than 64K, in a file.
    */
   
    nErr = CompoundTextClose (
            hCompound,          /* handle to CompoundText context */
            &hCDBuffer,         /* if result is a buffer, handle goes here */
            &dwBufferSize,
            szTempFileName,     /* if this creates a file, name goes here */
            MAXPATH );
             
    if (nErr != NOERROR)
    {
        printf("Error: unable to close compound text context.\n");
        CompoundTextDiscard (hCompound);
	    PrintAPIError (nErr);  
	    NotesTerm();
		return (1);
    }

    if (hCDBuffer == NULLHANDLE)
    {
        /* Compound Text returned in a file, not in a buffer */

        if (strlen( szTempFileName ) == 0)
        {
            printf( "Error: no buffer and no file created.\n");
      	    PrintAPIError (nErr);  
    	    NotesTerm();
    	    return (PKG_ADDIN);
        }
            
        if (rename(szTempFileName, szCDFileName) == 0)
        {
            printf("Error: unable to rename temp file '%s' to '%s'.\n", 
                    szTempFileName, szCDFileName );
      	    PrintAPIError (nErr);  
    	    NotesTerm();
    	    return (PKG_ADDIN);
        }
   
		NotesTerm();
		return (0); 

    }

    /* else - Compound text returned in a buffer  */

    pData = OSLock( char, hCDBuffer );

    if (pData == NULL)
    {
        printf( "Error: unable to lock compount text buffer.\n" );
		NotesTerm();
		return (0); 
    }

    pCDFile = fopen( szCDFileName, FOPEN_MODE );

    if (pCDFile == (FILE*)NULL)
    {
        printf( "Error: unable to open output file '%s'.\n", szCDFileName );
		NotesTerm();
		return (0); 
    }

    sBufferSize = (SIZE_T)dwBufferSize ;

    sRetVal = fwrite( pData, sizeof(BYTE), sBufferSize, pCDFile );

    if (sRetVal != sBufferSize)
    {
        printf( "Error: did not write %d bytes to output file.\n", 
                sBufferSize);
    }
    else
    {
        printf( "Wrote %d bytes to file %s.\n", sRetVal, szCDFileName );
    }

    if (fclose( pCDFile ))
    {
        printf("Error: unable to close output file '%s'.\n", szCDFileName);
    }
    
    OSUnlockObject( hCDBuffer );

    OSMemFree( hCDBuffer );

    printf("\nProgram completed successfully.\n");
	NotesTerm();
	return (0); 
}

/************************************************************************

    FUNCTION:   ProcessArgs

    INPUTS:     argc, argv - directly from the command line
   
    OUTPUTS:    text - gets rich text field text

*************************************************************************/

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                             char *text)
{ 
    int i;

    text[0] = '\0';

#ifndef MAC
    if (argc == 1)
    {       
#endif
      
      printf("Enter text to be included in rich text field:\n");
      gets(text);

#ifndef MAC
   }  
   else
   {
      for (i=1; i<argc; i++)
      {
        strcat(text, argv[i]);
        strcat(text, " ");
      }
   } /* end if */
#endif
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
