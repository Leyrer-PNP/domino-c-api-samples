/***************************************************************************

     SENDMAIL.H

***************************************************************************/

#define TITLE_LEN            30
#define ERR_STR_LEN         256
#define SENDMAILERROR       1

/* constants */
#define     READMAIL_BODY_LINELEN   40
char szDriveLetter_blank[] = "";
char szExtensionName_TMP[] = "TMP";
char szNewline_Terminate[] = "\r\n";

void  LNPUBLIC  ProcessArgs(int, char*[], char *, char *, char *, char *, char *);
int SendMailMainProc();
int QuitSendMail();
int SendMail();
void LNPUBLIC ConvertNewlinesToNulls(char *, int);
char *BuildErrStr(char *);
STATUS near pascal GetUniqueFileName(char *Drive, char *Ext,
                                        char *FileName);
STATUS PrintMail(char *szServerName);
