#
# makefile for Notes API sample program MGATE
# Windows 32-bit version using Microsoft Visual C++ .NET compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

outfilename = nmgate

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename).exe: main.obj inbound.obj outbound.obj nmgate.res
   $(link) $(linkdebug) $(conflags) main.obj inbound.obj outbound.obj notes0.obj notesai0.obj \
		nmgate.res $(conlibs) notes.lib user32.lib -out:$(outfilename).exe

# Update the resource file if necessary.
nmgate.res : mswin32.rc mgateerr.h
        rc -r -fo nmgate.res -DWIN32 -D_WIN32 /DNT mswin32.rc

# Update the object file if necessary.
main.obj : main.c mgate.h mgateerr.h
	$(cc) $(cdebug) $(cflags) $(cpuflags) $(cvars) /DNT main.c
	
inbound.obj : inbound.c mgate.h mgateerr.h
	$(cc) $(cdebug) $(cflags) $(cpuflags) $(cvars) /DNT inbound.c
	
outbound.obj : outbound.c mgate.h mgateerr.h
	$(cc) $(cdebug) $(cflags) $(cpuflags) $(cvars) /DNT outbound.c
