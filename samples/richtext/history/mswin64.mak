#
# makefile for Notes API sample program NSF_DUMP.
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

all: history.exe



history.exe	:	history.obj histwin.res

history.obj : history.c history.h histerr.h

# Update the resource file if necessary.
histwin.res : histwin.rc histwin.dlg histwin.ico history.h
        rc -r -fo histwin.res -DWIN64 -D_WIN64 /DNT histwin.rc
# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W4 $*.c

# Link command.
	
.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
