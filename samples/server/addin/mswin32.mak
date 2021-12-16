#
# makefile for Notes API sample program ADDIN
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

outfilename = naddin

# Update the executable file if necessary
$(outfilename).exe: addin.obj
	$(link) $(linkdebug) $(conflags) addin.obj notes0.obj notesai0.obj \
		$(conlibs) notes.lib user32.lib -out:$(outfilename).exe

# Update the object file if necessary.
addin.obj : addin.c
	$(cc) $(cdebug) $(cflags) $(cvars) $(cpuflags) /DNT addin.c
