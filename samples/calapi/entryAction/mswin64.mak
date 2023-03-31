#
# Makefile for Notes API sample program entryAction
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = entryAction



all: chair.exe OneAttendee.exe

chair.exe: chair.obj
	link $(LOPTIONS_1) -subsystem:console,5.0 -out:$@ chair.obj $(ENTRY_FLAG) \
	kernel32.lib  ws2_32.lib mswsock.lib advapi32.lib  notes.lib user32.lib

OneAttendee.exe: OneAttendee.obj
	link $(LOPTIONS_1) -subsystem:console,5.0 -out:$@ OneAttendee.obj $(ENTRY_FLAG) \
	kernel32.lib  ws2_32.lib mswsock.lib advapi32.lib  notes.lib user32.lib

chair.obj:
	!cl $(COPTIONS) -D_WINNT -D_WIN32_WINNT=0x0500 chair.c
    
OneAttendee.obj:
	!cl $(COPTIONS) -D_WINNT -D_WIN32_WINNT=0x0500  OneAttendee.c
