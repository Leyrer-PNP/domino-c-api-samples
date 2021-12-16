#ifndef __READFORM_H
#define __READFORM_H

/****************************************************************************

    PROGRAM:    readform

    FILE:       readform.h

    PURPOSE:    symbolic constants, types, and function prototypes

****************************************************************************/

#define TEXT_BUFFER_LENGTH     1024 

/*
 * Prototype of routine to locate form and get form information.
 */
 
STATUS LNPUBLIC ReadForm(char *pFormName);

/*
 * Prototype of routine to processes the body field.
 */
 
STATUS LNPUBLIC ReadBody(BLOCKID ValueBlockID,
                           DWORD dwLength,
                           char far *pOutputBuffer);

 
/*
 * Prototype of action routine.
 */
STATUS LNCALLBACK FormFields (char far *RecordPtr,
                              WORD RecordType,
                              DWORD RecordLength,
                              void far *pCtx);

#endif
