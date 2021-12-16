/*
 *		extcwin.c - Windows DLL framework for extension manager.
 */

	/* Windows header files */
#include "windows.h"

	/* HCL C API for Notes/Domino header files */
#include "global.h"

#/* Program specific include file */
#include "extconf.h"

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif

HINSTANCE	hDllInstance = (HINSTANCE) 0;

/*
 *		DLL Management
 */

#if defined (_WIN32)

/* Win32 DLL Startup/Shutdown routine */
DLL_EXPORT_PREFIX BOOL WINAPI DllMain (HANDLE    hInst,
                                       ULONG    dwReason,
                                       LPVOID   lpReserved) 
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            /* Save the instance handle */
            hDllInstance = hInst;
            break;

        case DLL_PROCESS_DETACH:
            /* Deregister extension manager callbacks */
            ExtClear ();
            break;
      }

      return (TRUE);
}

#endif


