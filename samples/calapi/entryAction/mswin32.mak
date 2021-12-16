#
# Makefile for Notes API sample program entryAction
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = entryAction

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF   

# Dependencies

all: chair.exe OneAttendee.exe

chair.exe: chair.obj
	$(link) $(linkdebug) $(conflags) -out:$@ chair.obj $(conlibs) \
	notes.lib user32.lib

OneAttendee.exe: OneAttendee.obj
	$(link) $(linkdebug) $(conflags) -out:$@ OneAttendee.obj $(conlibs) \
	notes.lib user32.lib

chair.obj:
	$(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) chair.c
    
OneAttendee.obj:
	$(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) OneAttendee.c
	
	
	
	
	
	