#
# makefile for Notes API sample program SERVLIST
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

proj = SERVLIST

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	

all: $(proj).exe

# Update the resource file if necessary.
$(proj).res : mswin32.rc $(proj).h
        rc -r -fo $(proj).res -DWIN32 -D_WIN32 /DNT mswin32.rc

# Update the object file if necessary.
$(proj).obj : $(proj).c $(proj).h
	$(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $(proj).c

# Update the executable file if necessary, and if so, add the resource
# back in.
$(proj).exe: $(proj).obj $(proj).res mswin32.def
	$(link) $(linkdebug) $(guiflags) $(proj).obj $(proj).res \
		$(guilibs) notes.lib -out:$(proj).exe
