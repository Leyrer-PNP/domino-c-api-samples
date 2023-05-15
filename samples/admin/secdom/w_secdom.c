/*************************************************************************

    PROGRAM:    SECDOM

    FILE:       W_SECDOM.C (Windows specific code)

    PURPOSE:    C API Sample program that illustrates how to create a
	             library that will, from the web, authenticate a Domino
					 user through his Operating System user account via DSAPI.
					
*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* W32 include files */
#include <windows.h>
#include <winbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <printLog.h>



/* ************************************************************* */
/* * Windows API for OS authentication.                        * */
/* ************************************************************* */
int separateUsernameAndDomainname(char *userName,char *separator,
                                  char **user, char **domain)
{
   *user=strtok(userName,separator);
   *domain=strtok(NULL,separator);
   return 0;

}

/* ************************************************************* */
/* * Windows API for OS authentication.                        * */
/* ************************************************************* */
int winAuthenticate(char *userName, char *domain, char *password)
{
   char  *lpMsgBuf;
   HANDLE phToken;
	
   PRINTLOG("\n Executing Windows-specific authentication for user %s in domain %s\n",userName,domain);

   if (LogonUser(userName,domain,password,LOGON32_LOGON_NETWORK,
                 LOGON32_PROVIDER_DEFAULT,&phToken))
   {
      PRINTLOG(" ** Successful return from Windows-specific authentication \n");
      return NOERROR;
   }
   else
   {
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0,
                    NULL);
      PRINTLOG("***** Error from Windows-specific authentication: ***\n");
      PRINTLOG("      %s\n",lpMsgBuf);
      LocalFree(lpMsgBuf);
      return -1;
   }

}

#ifdef __cplusplus
}
#endif
