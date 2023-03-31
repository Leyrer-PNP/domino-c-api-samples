#
# Makefile for Notes API sample program entrymanip
# Windows 64-bit version using Microsoft Visual Studio 2017 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = entrymanip


$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -subsystem:console,5.0 -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1) mswsock.lib
