#
# makefile for Notes API sample program mktable.
#		Windows 32-bit version
#
# This makefile is designed for use under Windows 32-bit
# using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows NT make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

# Master build target

all : mktable.exe

# Dependencies

mktable.exe  : mktable.obj cdrecord.obj cdtable.obj
mktable.obj  : mktable.c mktable.h
cdrecord.obj : cdrecord.c mktable.h
cdtable.obj  : cdtable.c mktable.h

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
    $(link) $(linkdebug) $(conflags) -out:$@ $** $(conlibs) \
      notes.lib user32.lib
