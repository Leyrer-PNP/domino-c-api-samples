#
# makefile for Notes API sample program NSF_DUMP.
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = NSF_DUMP


nsf_dump.exe : nsf_dump.obj dumpitem.obj dumpcd.obj cdfile.obj agents.obj navigate.obj mime.obj

nsf_dump.obj : nsf_dump.c nsf_dump.h

dumpitem.obj : dumpitem.c nsf_dump.h

dumpcd.obj : dumpcd.c nsf_dump.h

cdfile.obj : cdfile.c nsf_dump.h

agents.obj : agents.c nsf_dump.h

navigate.obj : navigate.c nsf_dump.h

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W4  $*.c

# Link command.
	
.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
