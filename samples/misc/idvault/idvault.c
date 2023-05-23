/*************************************************************************

   PROGRAM:    idvault

   FILE:       idvault.c

   PURPOSE:    This programe upload id file to ID vault,gets id file from ID vault,
               synch id file contents with the ID vault

   

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
#include "idvault.h"
#include "nsfdb.h"
#include "osfile.h"
#if defined(OS390)
#include "lapicinc.h"
#endif
#include "lapiplat.h"
#include "printLog.h"

/* Program declaration */
int main(int argc, char *argv[])
{
    /* Local data declarations */
         
    STATUS     error = NOERROR;             /* error code from API calls */
    KFHANDLE   hKFC = NULLKFHANDLE;
    KFHANDLE   hKFC2 = NULLKFHANDLE;
    DBHANDLE   hDB;

	char    *ServerName;
	char    *DomainName;
	char	DBName[]="names.nsf";
	char    DBPath[MAXPATH+1];
#ifdef OS400
	char	UserS1ID[] = "/DominoServer/LGCToolkit/people/s1.id";
	char	UserS2ID[] = "/DominoServer/LGCToolkit/people/s2.id";
	char	UserS3ID[] = "/DominoServer/LGCToolkit/people/s3.id";
	char	UserSSID[] = "/DominoServer/LGCToolkit/people/ss.id";
#else
	char	UserS1ID[] = "s1.id";
	char	UserS2ID[] = "s2.id";
	char	UserS3ID[] = "s3.id";
	char	UserSSID[] = "ss.id";
#endif
	char	UserNameS1[MAXUSERNAME] = "CN=s1/O=";
	char	UserNameS2[MAXUSERNAME] = "CN=s2/O=";
	char	UserNameS3[MAXUSERNAME] = "CN=s3/O=";
	char	UserNameSS[MAXUSERNAME] = "CN=ss/O=";
	char	PASSWORD[]="123456";
	DWORD	VaultFlags = 0;  
    
	if(argc != 3)
	{
		PRINTLOG( "\nUsage:  %s  <server name> <domain>\n", argv[0] );
		return (0);
	}

	/* init ... */
	ServerName = argv[1];
	DomainName = argv[2];	
	strcat(UserNameS1,DomainName);
	strcat(UserNameS2,DomainName);
	strcat(UserNameS3,DomainName);
	strcat(UserNameSS,DomainName);


    if (error = NotesInitExtended (argc, argv))
    {
      fprintf (stderr, "\nError initializing Notes.\n");
      return (1);
    }

	OSPathNetConstruct( NULL, ServerName,DBName,DBPath);


	/*Must be call NSFDbOpen() for init before execute other id vault functions.*/	
	NSFDbOpen (DBName, &hDB);
	

	error = SECKFMOpen (&hKFC, UserS1ID, PASSWORD, SECKFM_open_All, 0, NULL);
	if ( error ){
        PRINTERROR (error,"SECKFMOpen");
        NotesTerm();
        return (1);

	}
	/* Put ID file using the hKFC */
	error = SECidfPut (UserNameS1, 
			   PASSWORD,
			   UserS1ID,
			   &hKFC, 
			   ServerName,
			   0,
			   0,
			   NULL);
	
	if ( error ){
        PRINTERROR (error,"SECidfPut");
        NotesTerm();
        return (1);

	}
	PRINTLOG("Put ID file using the hKFC...OK\n");

	SECKFMClose (&hKFC, SECKFM_close_WriteIdFile, 0, NULL);

    error = SECKFMOpen (&hKFC, UserS2ID, PASSWORD, SECKFM_open_All, 0, NULL);
	if ( error ){
        PRINTERROR (error,"SECKFMOpen");
        NotesTerm();
        return (1);

	}
	/* Put ID file using local file name and hKFC - only hKFC should be used */
 	error = SECidfPut (UserNameS2, 
			   PASSWORD,
			   NULL, 
			   &hKFC, 
			   ServerName, 
			   0, 
			   0,
			   NULL);
	if ( error ){
        PRINTERROR (error,"SECidfPut");
        NotesTerm();
        return (1);

	}

	PRINTLOG("Put ID file using local file name and hKFC - only hKFC should be used...OK\n");
	SECKFMClose (&hKFC, SECKFM_close_WriteIdFile, 0, NULL);

    error = SECKFMOpen (&hKFC, UserS3ID, PASSWORD, SECKFM_open_All, 0, NULL);
	if ( error ){
        PRINTERROR (error,"SECKFMOpen");
        NotesTerm();
        return (1);

	}
	
	/* Put ID file using local file name */
	error = SECidfPut (UserNameS3, 
			   PASSWORD,
			   UserS3ID, 
			   NULL, 
			   ServerName, 
			   0, 
			   0,
			   NULL);

	if ( error ){
        PRINTERROR (error,"SECidfPut");
        NotesTerm();
        return (1);

	}

	PRINTLOG("Put ID file using local file name...OK\n");
	SECKFMClose (&hKFC, SECKFM_close_WriteIdFile, 0, NULL);

	/* Get id file to local file name and hKFC */
	error = SECidfGet (UserNameS1, 
			PASSWORD,
			UserS1ID, 
			&hKFC2, 
			ServerName, 
			0,
			0,
			NULL);
	if ( error ){
        PRINTERROR (error,"SECidfGet");
        NotesTerm();
        return (1);

	}
	PRINTLOG("Get id file to local file name and hKFC...OK\n");


	SECKFMClose (&hKFC2, SECKFM_close_WriteIdFile, 0, NULL);


	/* Get id file to just hKFC*/
	error = SECidfGet (UserNameS2, 
			PASSWORD,
			NULL, 
			&hKFC2, 
			ServerName, 
			0,
			0,
			NULL);
	if ( error ){
        PRINTERROR (error,"SECidfGet");
        NotesTerm();
        return (1);

	}

	PRINTLOG("Get id file to just hKFC...OK\n");
	SECKFMClose (&hKFC2, SECKFM_close_WriteIdFile, 0, NULL);



	/* Get id file to just local file name */
	error = SECidfGet (UserNameS3, 
			PASSWORD,
			UserS3ID,  
			NULL, 
			ServerName, 
			0,
			0,
			NULL);
	if ( error ){
        PRINTERROR (error,"SECidfGet");
        NotesTerm();
        return (1);

	}

	PRINTLOG("Get id file to just local file name...OK\n");

	SECKFMOpen (&hKFC2, UserS1ID, PASSWORD, SECKFM_open_All, 0, NULL);

	/* Sync the hKFC with the ID file in the Vault and write new hKFC */
	error = SECidfSync (UserNameS1, PASSWORD,
			NULL,
			&hKFC2, 
			ServerName, 
			0, 
			0,
			NULL, 
			&VaultFlags);
	if ( error ){
        PRINTERROR (error,"SECidfSync");
        NotesTerm();
        return (1);

	}
	PRINTLOG("Sync the hKFC with the ID file in the Vault and write new hKFC...OK\n");

	/* Sync the ID file with the ID file in the Vault */
	error = SECidfSync (UserNameS1, PASSWORD,
			UserS1ID, 
			NULL,
			ServerName, 
			0,
			0, 
			NULL, 
			&VaultFlags);
	if ( error ){
        PRINTERROR (error,"SECidfSync");
        NotesTerm();
        return (1);

	}
	PRINTLOG("Sync the ID file with the ID file in the Vault ...OK\n");

	/* Sync the hKFC with the ID file in the Vault and hKFC */
	error = SECidfSync (UserNameS1, PASSWORD,
			UserS1ID, 
			&hKFC2, 
			ServerName, 
			0,
			0,
			NULL, 
			&VaultFlags);
	if ( error ){
        PRINTERROR (error,"SECidfSync");
        NotesTerm();
        return (1);

	}
	PRINTLOG("Sync the hKFC with the ID file in the Vault and hKFC...OK\n");

	SECKFMClose (&hKFC2, SECKFM_close_WriteIdFile, 0, NULL);


    /* Terminate Domino and Notes. */



	NSFDbClose(hDB);

    NotesTerm();

    /* End of intro program. */
	PRINTLOG("\nProgram completed successfully.\n");
    return (0);
}

