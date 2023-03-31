#
# makefile for Notes API sample program convertattach/DBRECS.
#
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#targets
ALL: convertattach.EXE


# Dependencies

convertattach.EXE: convertattach.OBJ

convertattach.OBJ: convertattach.C


# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W3 $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) notes.lib kernel32.lib user32.lib
