# makefile for Notes API sample program mktable.
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>



all : mktable.exe

# Dependencies

mktable.exe  : mktable.obj cdrecord.obj cdtable.obj
mktable.obj  : mktable.c mktable.h
cdrecord.obj : cdrecord.c mktable.h
cdtable.obj  : cdtable.c mktable.h

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
