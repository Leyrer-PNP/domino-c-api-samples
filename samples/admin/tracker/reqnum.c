/****************************************************************************

    PROGRAM: reqnum

    FILE:    reqnum.c

    SYNTAX:  reqnum

    PURPOSE: create a file "reqnum.dat" containing NUMBER = 1
             for use by HCL C API for Notes/Domino sample program tracker. 

    DESCRIPTION:
   
    HCL C API for Notes/Domino  Sample program tracker requires a data file 
	with name "reqnum.dat" to manage a unique sequential request number
    service. Under Windows, this file resides in "c:\" by default.
    This program creates this file containing the number 1.

****************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* C include files */
#include <stdio.h>
#include <errno.h>

/* HCL C API for Notes/Domino include files */
#include <global.h>                     /* NUMBER */
#include <names.h>                      /* MAXUSERNAME */
#include <nsfdb.h>                      /* DBHOOKVEC */
#include <nsfdata.h>                    /* LIST */
#include <printLog.h>

/* Application-specific include files */
#include "tracker.h"                    /* REQNUM_FILENAME */

/************************************************************************

    FUNCTION:   main

    PURPOSE:    Create the "next Request Number to Use" file reqnum.dat.
                Set the number to 1.

************************************************************************/

int main (int argc, char *argv[])
{
    FILE       *pReqNumFile;
    NUMBER      ReqNumber;
    size_t      count;

#if defined(OS400)
    pReqNumFile = fopen(REQNUM_FILENAME, "wb ccsid=819");
#else
    pReqNumFile = fopen(REQNUM_FILENAME, "w");
#endif


    if (pReqNumFile == (FILE*)NULL)
    {
        PRINTLOG("Error: unable to open file '%s': %d.\n",
                            REQNUM_FILENAME, errno);
        return 0;
    }

    ReqNumber = 1;
    count = fwrite(&ReqNumber, sizeof(NUMBER), 1, pReqNumFile);
    fclose(pReqNumFile);

    if (count != 1)
    {
        PRINTLOG("Error: error writing file '%s' : %d.\n",
                            REQNUM_FILENAME, errno);
    }

    return 0;
}
