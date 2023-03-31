#
# Makefile for HCL C API for Domino and Notes sample program, environment
# Windows 64-bit version
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = environment

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W4  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG) $(LIBS_1)
