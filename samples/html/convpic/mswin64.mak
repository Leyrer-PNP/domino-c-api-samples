#
# makefile for Notes API sample program convertpic/DBRECS.
#
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#targets
ALL: convertpic.EXE

# Dependencies

convertpic.EXE: convertpic.OBJ

convertpic.OBJ: convertpic.C


# Compilation command.

.C.OBJ:
    !cl $(COPTIONS)  -W4 $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
