#
# makefile for Notes API sample program ATTACH
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = ATTACH

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

# Dependencies

attach.EXE:   attach.OBJ cdrecord.obj
attach.OBJ:   attach.C
cdrecord.OBJ: cdrecord.C

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
attach.exe: attach.obj cdrecord.obj
    $(link) $(linkdebug) $(conflags) attach.obj cdrecord.obj \
            $(conlibs) $(olelibs) user32.lib notes.lib -out:attach.exe
