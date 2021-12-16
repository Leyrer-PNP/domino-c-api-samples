#
# makefile for Notes API sample program THREADS
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

outfilename = nthreads

# Update the executable file if necessary
$(outfilename).exe: threads.obj
	$(link) $(linkdebug) $(conflags) threads.obj notes0.obj notesai0.obj \
		$(conlibsmt) notes.lib user32.lib -out:$(outfilename).exe

# Update the object file if necessary.
threads.obj : threads.c
	$(cc) $(cdebug) $(cflags) $(cvarsmt) $(cpuflags) /DNT threads.c
