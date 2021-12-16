
/*
 * IMPORT.H  local include file for the large graphic import example
 */

#ifndef __IMPORT_H
#define __IMPORT_H



#define CD_BUFFER_LENGTH       64000
#define CD_HIGH_WATER_MARK     40000
 
/*
 *  Error code and error message text to return if the call to 
 *  AppendImportItem fails.
 */
 
#define ERR_APPEND_RICHTEXT_ERROR  PKG_ODS+50     
errortext (ERR_APPEND_RICHTEXT_ERROR, "Error appending rich text item.");


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
#endif




