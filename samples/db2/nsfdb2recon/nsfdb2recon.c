/****************************************************************************
        STATUS LNPUBLIC NSFDB2ReconnectNotesDatabase(
                                        const char far *FullPath,
                                        const char far *String1,
                                        DWORD  nsfid,
                                        DWORD  flags);
    PROGRAM:    nsfdb2recon

    FILE:       nsfdb2recon.c

    SYNTAX:     nsfdb2recon <DB_NAME> <server_name> <nsfID>

    PURPOSE:    Make sure that Domino can access NSFDB2 data for FullPath or 
                    rename the NSFDB2 database within the Domino server 
   Algorithm:
               If FullPath  or nsfid  is provided, scan all properties tables in the tablespace specified 
                      by string1, until we find one in a schema that claims to containing data for FullPath. or nsfid
                      If one row is found:
                      1) Extract the NSFID from the properties table found in the above step.
                        2) Update Domino's CATALOG table.
                        3) Make sure link file on  Domino server's file system exists.
                        4) If DB2BACKUP_FASTCONNECT is not specified as an option, restore function and trigger 
                        definitions for access tables & views, removing old values if necessary.
                        5) If DB2BACKUP_NOREPLICA is specified in flags, regenerate the REPLICA ID of this database.
                Else return ERR_DB2NSF_IDENTIFIER_INVALID.
            If FullPath and nsfid was not provided
            Reconnect NSFDB2 databases discovered in the DB2 tablespace by scanning PROPERTIES table in groupName for 
            NSFDB2 data and verifying the data actually exists.  Reconnect the NSFDB2 database as described above.

****************************************************************************/

/* OS and C include files */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* HCL C API for Notes/Domino include files */

#include <global.h>
#include <nsfdb.h>
#include <osmisc.h>
#include <nsfnote.h>
#include <log.h>
#include <osmem.h>
#include <osfile.h>
#include <printLog.h>

#define Recover_DB                        "recover.nsf"                /*recovered DB name*/



/************************************************************************

    FUNCTION:   Main

    PURPOSE:    HCL C API for Notes/Domino subroutine

*************************************************************************/

int main(int argc, char *argv[])
{
        char                *DB_NAME;
        char                *server_name;
        char                *c_nsfID;
        DWORD               nsfID;
        char                fullpath[100];            /*Relative to the Domino server's data directory.  */
                                                      /*This does not have to be the same as the original path*/
        char                Group_Name[100];          /*NSFDB2 group (DB2 tablespace) containing the NSFDB2 database you wish to reconnect*/

        STATUS              error = 0;                /* return code from API calls */
        DBHANDLE            db_handle = NULL;         /* handle to database */
           
        DWORD               flags = 0;                /*flags  -  optional. See DB2BACKUP_xxx.*/
        FILE                *flog = NULL;
        TIMEDATE            retDataMod;
        TIMEDATE            retNonDataMod;
         
        char                buffer[200];
        DWORD               dwBuffer;
        DWORD               size;
        int                 i;
        DWORD infotype[] = {        DB2NSF_INFO_IS_DB2_BACKED,      /*1     Is database DB2-backed: 1/0*/
                                    DB2NSF_INFO_SCHEMA_NAME,        /*2     Group schema containing NSFDB2 data*/
                                    DB2NSF_INFO_TABLESPACE_NAME,    /*3     Tablespace name*/
                                    DB2NSF_INFO_TSID,               /*4     Tablespace ID*/
                                    DB2NSF_INFO_USERSCHEMA_NAME,    /*5     Schema containing DAVs*/
                                    DB2NSF_INFO_CLASS_DESC          /*6     NSFDB2 grouping class description*/
                           }; 
        char* infoString[] = {  "DB2NSF_INFO_IS_DB2_BACKED",        /*1     Is database DB2-backed: 1/0&*/
                                "DB2NSF_INFO_SCHEMA_NAME",          /*2     Group schema containing NSFDB2 data*/
                                "DB2NSF_INFO_TABLESPACE_NAME",      /*3     Tablespace name*/
                                "DB2NSF_INFO_TSID",                 /*4     Tablespace ID*/
                                "DB2NSF_INFO_USERSCHEMA_NAME",      /*5     Schema containing DAVs*/
                                "DB2NSF_INFO_CLASS_DESC"            /*6     NSFDB2 grouping class description*/
                             };        

        /*  Command line parameters. */
        if (argc != 4)
        {
            PRINTLOG ("\nUsage:  %s  <DB_NAME> <server_name> <nsfID>  \n", argv[0]);
            return (NOERROR);
        }

        DB_NAME = argv[1];
        server_name = argv[2];
        c_nsfID = argv[3];
        nsfID = (DWORD)atoi(c_nsfID);

        error = NotesInitExtended (argc, argv);
        if (error)
        {
            PRINTLOG(" ========Error: Unable to initialize Notes.\n");
            return (1);
        }
        
        PRINTLOG("========init notes successfully!\n");

        if (strlen(server_name))
        {
             if (error = OSPathNetConstruct(NULL, 
                                            server_name,
                                            DB_NAME, 
                                            fullpath))
             {
                   PRINTLOG("========ERR: Can not Construct OSPathNet\n");
                   PRINTERROR(error,"OSPathNetConstruct");
                   goto EXIT;
             }
        }
        else
        {
             strcpy(fullpath, DB_NAME);
        }
        
        if ( error = NSFDbOpenExtended (fullpath, 
                                        0, 
                                        NULLHANDLE, 
                                        NULL ,
                                        &db_handle,
                                        &retDataMod,
                                        &retNonDataMod))
        {
             PRINTLOG("========error when open DB\n");
             PRINTERROR(error,"NSFDbOpenExtended");
             goto EXIT;
        }
        PRINTLOG("\n========open the database:%s successfully\n", fullpath); 
        
        flags = DB2BACKUP_FASTCONNECT;

        for( i=0; i<6; i++)
        {
                
                size = sizeof(buffer);
                if( (i==0) | (i==3) )
                {
                        size = sizeof(DWORD);
                        if(error = NSFDB2GetInfo(db_handle, infotype[i],
                                              &dwBuffer,&size))
                        {
                                PRINTLOG("\n========Error in getting DB2INFO %d, error is:", infotype[i]);
                                PrintAPIError(error);
                                goto EXIT;                                
                        }
                        else
                        {
                                PRINTLOG("\n========Get DB2 info:%s,return value is %d\n", infoString[i], dwBuffer);
                        }
                }
                else
                {
                        size = sizeof(buffer);
                        if(error = NSFDB2GetInfo(db_handle, infotype[i],
                                      buffer,&size))
                        {
                                PRINTLOG("\n========Error in getting DB2INFO %d, error is:", infotype[i]);
                                PRINTERROR(error,"NSFDB2GetInfo");
                                goto EXIT;                                
                        }
                        else
                        {
                                if( i == 2)
                                {
                                        memcpy(Group_Name, buffer, sizeof(buffer));
                                }
                        
                                PRINTLOG("\n========Get DB2 info:%s,return value is %s\n", infoString[i], buffer);
                        }
                }
        }
           /*Test the api*/
        PRINTLOG("\n========test api NSFDB2ReconnectNotesDatabase\n");
        PRINTLOG("        with domino data releative path and no nsfID set\n");

        error = NSFDB2ReconnectNotesDatabase (  DB_NAME,
                                                (const char far *)Group_Name,
                                                 DB2BACKUP_NONSFID,
                                                 flags);
        if( error )
        {
                PRINTLOG("\n========Error in execute NSFDB2ReconnectNotesDatabase!\n");
                PRINTERROR(error,"NSFDB2ReconnectNotesDatabase");
                goto EXIT;
        }
        else 
        {
                PRINTLOG("\nexecute  successfully\n");
        }
        
        /*close the db , and reconnect the link to another NSF file with the API 
        NSFDB2ReconnectNotesDatabase, and then delete the original one, if the original
        link file can be deleted, the means the recover process is done successfully*/
        if(error = NSFDbClose (db_handle))
        {
              PRINTERROR(error,"NSFDbClose");
              goto EXIT;
           
        }

        PRINTLOG("\n========test to recover the db, with the option nsfid specified\n");
  
        error = NSFDB2ReconnectNotesDatabase (  Recover_DB,
                                                (const char far *)Group_Name,
                                                 nsfID,
                                                 flags);
        if( error )
        {
            PRINTLOG("\n========Error: can not recover!\n");
            PRINTERROR(error,"NSFDB2ReconnectNotesDatabase");
            goto EXIT;
        }
        else 
        {
            PRINTLOG("\n========NSFDB2 db recovered successfully!!\n");
        }
                        
        if(error = NSFDbDelete(fullpath))
        {
            PRINTLOG("\n========Error: can not execute NSFDBDelete!\n");
            PRINTERROR(error,"NSFDbDelete");
            goto EXIT;
        }
        else 
        {
            PRINTLOG("\n========execute NSFDBDelete successfully!!\n");
        }

        PRINTLOG(" Terminating Notes...\n");
        NotesTerm();

        /* End of main routine */
        PRINTLOG("\n========Program completed successfully.\n");
        return (0);
 
 EXIT:
        NotesTerm();
        return (1);
}
