BOOL LNPUBLIC PutText(
  BYTE  *pbRTItem,
  LPSTR  pText,
  DWORD  dwLength,
  DWORD *pdwRTLength);

BOOL LNPUBLIC PutOLEBegin(
  BYTE  *pbRTItem,
  char  *pszAttachmentName,
  char  *pszClassName,
  DWORD  dwLength,
  DWORD *pdwRTLength);

BOOL LNPUBLIC PutOLEEnd(
  BYTE  *pbRTItem,
  DWORD  dwLength, 
  DWORD *pwRTLength);
