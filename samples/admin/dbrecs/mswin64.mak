#
# makefile for Notes API sample program DBRECS.
#
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

#targets
ALL: dbrecs.EXE 


##

# Dependencies

dbrecs.EXE: dbrecs.OBJ win32io.OBJ

dbrecs.OBJ: dbrecs.C win32io.h

win32io.OBJ: win32io.C win32io.h

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG) $(LIBS_1)
