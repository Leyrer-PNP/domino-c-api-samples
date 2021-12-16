#if defined(OS400)
#pragma convert(850)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* OS and C include files */

#include <stdio.h>
#include <string.h>

/* HCL C API for Notes/Domino include files */

#include <global.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <nsfsearc.h>
#include <osmem.h>
#include <nsferr.h>
#include <osmisc.h>

/* Local include files */

#include "user_def.h"

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
        printf("Enter name of database: ");
        fflush(stdout);
        gets(db_filename);
    }
    else
    {   
         strcpy(db_filename, argv[1]);
    } /* end if */
} /* ProcessArgs */

#ifdef __cplusplus
}
#endif