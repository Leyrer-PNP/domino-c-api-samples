#
# makefile for Notes API sample program HISTORY
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <NTWIN32.MAK>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

all: history.exe

# Update the resource file if necessary.
histwin.res : histwin.rc histwin.dlg histwin.ico history.h
        rc -r -fo histwin.res -DWIN32 -D_WIN32 /DNT histwin.rc

# Update the object file if necessary.
history.obj : history.c history.h histerr.h
	$(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) history.c

# Update the executable file if necessary, and if so, add the resource
# back in.
history.exe: history.obj histwin.res
	$(link) $(linkdebug) $(guiflags) history.obj histwin.res \
		$(guilibs) notes.lib -out:history.exe
