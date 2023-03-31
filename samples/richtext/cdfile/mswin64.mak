#
# makefile for Notes API sample program CDFREAD and CDFWRITE
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#

# Master build target

all : cdfread.exe cdfwrite.exe

	
cdfread.exe : cdfread.obj

cdfread.obj : cdfread.c cdfile.h

cdfwrite.exe : cdfwrite.obj

cdfwrite.obj : cdfwrite.c cdfile.h

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W4  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
