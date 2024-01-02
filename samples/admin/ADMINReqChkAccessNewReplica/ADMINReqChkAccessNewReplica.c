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

PROGRAM:    ADMINReqDeleteInACL

FILE:       ADMINReqDeleteInACL.c

PURPOSE:    This program will creates a "Check Access for New Replica Creation" request in the Administration Requests database (admin4.nsf).

SYNTAX:     ADMINReqChkAccessNewReplica  <source server name> <source database name> <destination server name> <destination full path database name>

*******************************************************************************************************************************************/
#if defined(OS400)
#pragma convert(850)
#endif

/* OS and C include files */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* HCL C API for Notes/Domino include files */

#include "global.h"
#include "nsfdb.h"
#include "nsfdata.h"
#include "osmisc.h"
#include "osfile.h"
#include "reg.h"
#include "adminp.h"

#include <osmem.h>
#include <osenv.h>
#include <nsfnote.h>
#include <editods.h>
#include <stdnames.h>
#include <mailserv.h>
#include <mail.h>
#include <textlist.h>

#if defined(CAPI_TESTING) 
#include "printlog.h" 
#else
 #define PRINTLOG printf 
 #define PRINTERROR(api_error, api_name) {\
 char    szErrorText[256] = { 0 };\
 OSLoadString(NULLHANDLE, ERR(api_error), szErrorText, sizeof(szErrorText));\
 fprintf(stderr, "[ERROR]:%s:%d:%s - %s", __FILE__,__LINE__,api_name,szErrorText); }
#endif 

/* NOTE: This code MUST be the LAST file included so that ascii versions of the system APIs are used     */
#if defined(OS390) && (__STRING_CODE_SET__==ISO8859-1 /* If os390 ascii compile                          */)     
#include <_Ascii_a.h>
#endif

/* conestant types */
 #define     SrcServer_name_LEN         80
 #define     STDIN                   stdin

/* function prototypes */

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                               char *SrcServer_name, char *SrcDb_name ,char *DesServer_name, char *DesDb_name);

/* Program declaration */
int main(int argc, char *argv[])
{
    /* Local data declarations */

    DBHANDLE    db_handle = NULLHANDLE;         /* database handle */
    DBHANDLE    srcDB_handle = NULLHANDLE;         /* database handle */
    STATUS      error = NOERROR;                /* error code from API calls */
    char        chAuthorName[MAXUSERNAME+1] = {0};    /* author's name */
    ADMINReqParams arpPtr;                      /* ADMINReqParams structure */
    DBREPLICAINFO  Replicainfo;
    int         argNum = 0;
    char        szSourceServer[SrcServer_name_LEN] = {0};      /* server names are < 80 chars */
    char        szSourceDBName[MAXPATH] = {0};
    char        szDestinationServer[SrcServer_name_LEN] = {0};      /* server names are < 80 chars */
    char        szDestinationDBName[MAXPATH] = {0};
    char        *chSrcServer = NULL;     /* source Server name */
    char        *chSrcPathName = NULL;   /* source file name */
    char        *chDesServer = NULL;     /* destination Server name */ 
    char        *chDesPathName = NULL;   /* destination file name */
    char        admin4db[] = "admin4.nsf";
    char        chTitle[]= "test";      /* Title of the database being replicated */
    
    char        admin4DBFullPath[MAXPATH+1] = {0};
    char        srcDBFullPath[MAXPATH+1] = {0};
    
    chSrcServer = szSourceServer;
    chSrcPathName = szSourceDBName;
    chDesServer = szDestinationServer;
    chDesPathName = szDestinationDBName;

    
    ProcessArgs(argc, argv, chSrcServer, chSrcPathName, chDesServer, chDesPathName);

    /* Process input arguments */
    error = NotesInitExtended (argc, argv);
    
    /* Initialize the notes */
    if (error)
    {
        PRINTERROR(error, "NotesInitExtended");
        return(1);
    }
    
    /* Construct the path for the admin request file */
    if (error = OSPathNetConstruct(NULL, chSrcServer, admin4db, admin4DBFullPath))
    {
        PRINTERROR (error,"OSPathNetConstruct");  
        NotesTerm();
        return (1);
    }
    
    /* Open the database. */
    if (error = NSFDbOpen (admin4DBFullPath, &db_handle))
    {
        PRINTERROR (error,"NSFDbOpen");
        NotesTerm();
        return (1);
    }
    
    /* Construct the path for the srcDBFullPath request file */
    if (error = OSPathNetConstruct(NULL, chSrcServer,chSrcPathName, srcDBFullPath))
    {
        PRINTERROR (error,"OSPathNetConstruct");  
        NotesTerm();
        return (1);
    }
    
    /* Open the database. */
    if (error = NSFDbOpen (srcDBFullPath, &srcDB_handle))
    {
        PRINTERROR (error,"NSFDbOpen");
        NotesTerm();
        return (1);
    }
    
    /* Copy Replica info and provide new Replica ID as required. */
    if (error = NSFDbReplicaInfoGet (srcDB_handle, &Replicainfo))
    {
        NSFDbClose (srcDB_handle);
        NotesTerm();
        return (1);
    }
    
    /* get the mail server name */
    if (error = SECKFMGetUserName(chAuthorName))
    {
        PRINTERROR(error, "SECKFMGetUserName");
        NSFDbClose (db_handle);
        NotesTerm();
        return (1);
    }
    
    memset(&arpPtr, 0x00, sizeof(arpPtr));
    
    if (error = ADMINReqChkAccessNewReplica(db_handle,
                                            chAuthorName,
                                            chSrcServer,
                                            srcDBFullPath,
                                            chTitle,
                                            &Replicainfo,
                                            chDesServer,
                                            chDesPathName,
                                            &arpPtr,
                                            sizeof(arpPtr)))
    {
        PRINTERROR(error, "ADMINReqChkAccessNewReplica");
        NotesTerm();
        NSFDbClose(db_handle);
        return (1);
    }
    
    PRINTLOG("\n!!! ADMINReqChkAccessNewReplica Processed Sucessfully !!! \n ");
    
    /* Close the srcdatabase. */
    if (error = NSFDbClose (srcDB_handle))
    {
        PRINTERROR (error,"NSFDbClose");
        NotesTerm();
        return (1);
    }

    /* Close the database. */
    if (error = NSFDbClose (db_handle))
    {
        PRINTERROR (error,"NSFDbClose");
        NotesTerm();
        return (1);
    }

    /* Terminate Domino and Notes. */
    NotesTerm();
    /* End of program. */
    return (0);

}

/************************************************************************

    FUNCTION:   ProcessArgs

    INPUTS:     argc, argv - directly from the command line

    OUTPUTS:    SrcServer_name - source server name obtained from command line or
                              from prompt.
                SrcDb_name -  source database name obtained from command line or
                              from prompt.
                DesServer_name   - destination server name obtained from command line or
                              from prompt. 
                DesDb_name  - destination database name obtained from command line or
                              from prompt.
*************************************************************************/

void  LNPUBLIC  ProcessArgs (int argc, char *argv[],
                               char *SrcServer_name, char *SrcDb_name ,char *DesServer_name, char *DesDb_name)
{
    if (argc != 5)
    {

        printf("Enter Source Server name: ");
        fflush (stdout);
        fgets(SrcServer_name, SrcServer_name_LEN, STDIN);
        printf("\n");
        printf ("Enter source file name:  ");
        fflush (stdout);
        fgets(SrcDb_name, MAXPATH-1, STDIN);
        printf("\n");
        printf ("Enter destination Server name:  ");
        fgets(DesServer_name, MAXPATH-1, STDIN);
        printf("\n");
        printf ("Enter destination fullPath file name:  ");
        fgets(DesDb_name, MAXPATH-1, STDIN);
        printf("\n");
    }
    else
    {
        memset(SrcServer_name, '\0', SrcServer_name_LEN);    
        strncpy(SrcServer_name, argv[1], SrcServer_name_LEN-1);
        memset(SrcDb_name, '\0', MAXPATH);    
        strncpy(SrcDb_name, argv[2], MAXPATH-1);
        memset(DesServer_name, '\0', MAXPATH);    
        strncpy(DesServer_name, argv[3], sizeof(DesServer_name));
        memset(DesDb_name, '\0', MAXPATH);    
        strncpy(DesDb_name, argv[4], sizeof(DesDb_name));
    } /* end if */
} /* ProcessArgs */
