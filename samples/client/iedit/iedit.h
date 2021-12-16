/****************************************************************************

    PROGRAM: iedit

    FILE:    iedit.h

    PURPOSE: Symbolic constants and function prototypes.

****************************************************************************/

/* Symbolic constants */
#define ERR_STR_LEN                 127
#define IEDIT_BUFFSZ                1024
#define ERR_IEDIT_EOF               PKG_ADDIN+1

/* Function prototypes */
VOID   LNPUBLIC IeditMessageBox( char * szMessage, STATUS error );
STATUS LNPUBLIC IeditFileOpenForRead( char * FileName, int * phRetFile );
STATUS LNPUBLIC IeditFileCreate( char * FileName, int * phRetFile );
STATUS LNPUBLIC IeditFileOpenForWrite( char * FileName, int * phRetFile );
STATUS LNPUBLIC IeditFileWrite( int hFile, char *pData, WORD wCount );
STATUS LNPUBLIC IeditFileRead( int hFile, char *pData, WORD wCount, WORD *pwRead );
STATUS LNPUBLIC IeditFileSeek( int hFile, DWORD dwPos );
STATUS LNPUBLIC IeditFileClose( int hFile );
STATUS LNPUBLIC IeditFileDelete( char * FileName );

