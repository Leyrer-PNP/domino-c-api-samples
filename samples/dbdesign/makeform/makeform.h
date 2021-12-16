/*
 * MAKEFORM.H
 */

#ifndef __MAKEFORM_H
#define __MAKEFORM_H

#define CD_BUFFER_LENGTH       64000

/* forward declarations for CD manipulation functions  */

BOOL LNPUBLIC PutPara (char far * far *ppBuf,
                         WORD wLength);
                               
BOOL LNPUBLIC PutPabDef(char far * far *ppBuf,
                         WORD wPabDefNumber,
                         WORD wLength,
                         WORD wJustifyMode);
                                
BOOL LNPUBLIC PutPabRef(char far * far *ppBuf,
                          WORD wPabDefNumber,
                          WORD wLength);
                                
BOOL LNPUBLIC PutText(char far * far *ppBuf,
                        char far * pText,
                        WORD wLength);

STATUS LNPUBLIC CreateForm(void);
STATUS LNPUBLIC CreateSubForm(void);
STATUS LNPUBLIC CreateFrame(void);
STATUS LNPUBLIC GetNoteLinkInfo(void);

char far * ImportGifImage(char *ImageFileName, char far *pBuf, WORD width, WORD height);
char far * AddFrame(char far * far *pBuf, char far *pBufferStart);
char far * AddNoteLink(char far * far *pBuf, char far *pBufferStart);
                            
#endif




