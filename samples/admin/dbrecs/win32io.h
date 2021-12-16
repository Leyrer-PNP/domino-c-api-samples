/****************************************************************************

 PROGRAM: dbbackup/dbrecs

 FILE:    win32io.h

 PURPOSE: Symbolic constants and function prototypes.

****************************************************************************/

/* Function prototypes */
STATUS LNPUBLIC SysFileOpenRead( char * FileName, int * phRetFile );
STATUS LNPUBLIC SysFileCreate( char * FileName, int * phRetFile );
STATUS LNPUBLIC SysFileWrite( int hFile, char *pData, DWORD wCount );
STATUS LNPUBLIC SysFileRead( int hFile, char *pData, DWORD wCount);
STATUS LNPUBLIC SysFileSeek( int hFile, DWORD dwPos, DWORD Loc );
STATUS LNPUBLIC SysFileClose( int hFile );
STATUS LNPUBLIC SysFileDelete( char * FileName );
STATUS LNPUBLIC SysFileCreateDirectory ( char * lFile );
STATUS LNPUBLIC SysFileCopy ( char * Source, char * Target, BOOL exist );
char * print_api_error (STATUS api_error);
