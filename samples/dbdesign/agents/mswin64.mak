#
# makefile for Notes API sample program AGENTS
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>


#Master build target

all : agents.exe ragents.exe

# Dependencies

agents.exe  : agents.obj
agents.obj  : agents.c
ragents.exe : ragents.obj
ragents.obj : ragents.c


# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W3  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) kernel32.lib notes.lib
