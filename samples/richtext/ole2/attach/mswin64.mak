#
# makefile for Notes API sample program ATTACH
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = ATTACH


# Dependencies

attach.EXE:   attach.OBJ cdrecord.obj
attach.OBJ:   attach.C
cdrecord.OBJ: cdrecord.C

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
attach.exe: attach.obj cdrecord.obj
	link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1) ole32.lib oleaut32.lib OLDNAMES.lib uuid.lib
