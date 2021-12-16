#
# makefile for Notes API sample program STATDEMO
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

outfilename = nstatdemo

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename).exe: statdemo.obj statdemo.res
	$(link) $(linkdebug) $(conflags) statdemo.obj notes0.obj notesai0.obj \
		statdemo.res $(conlibs) notes.lib user32.lib -out:$(outfilename).exe

# Update the resource file if necessary.
statdemo.res : statdemo.rc statdemo.h
        rc -r -fo statdemo.res -DWIN32 -D_WIN32 /DNT statdemo.rc

# Update the object file if necessary.
statdemo.obj : statdemo.c statdemo.h
	$(cc) $(cdebug) $(cflags) $(cpuflags) $(cvars) /DNT statdemo.c
