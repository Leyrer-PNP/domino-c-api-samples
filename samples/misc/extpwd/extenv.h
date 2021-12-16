/*
 *      extenv.h - Environment header file for Extension 
 *                 Manager password program
 *
 *      This file declares the platform-specific functions used to build the
 *      Extension Manager password dialog box DLL.
 */

#ifndef _EXTENV_H_
#define _EXTENV_H_

/*
 *      Function Declarations
 */

#if defined (NT)

        /* Win32 DLL Startup/Shutdown routine 
        DLL_EXPORT_PREFIX BOOL WINAPI LibMain (
                HANDLE          hInst,
                ULONG           whyCalled,
                LPVOID          lpReserved
        );
*/
#endif

#if defined(NT) 

        /* Windows dialog proc to obtain password */
BOOL CALLBACK PasswordDlgProc (
        HWND            hWndDlg,
        UINT            uMsg,
        WPARAM          wParam,
        LPARAM          lParam
);

#endif

#endif
