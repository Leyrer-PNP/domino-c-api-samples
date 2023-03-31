#
# Makefile for HCL C API for Domino and Notes sample program, ACLS
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>
PROGNAME = ACLS

# The name of the program.

# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.
.C.OBJ:
    !cl $(COPTIONS)  $*.c

.OBJ.EXE:
    link $(LOPTIONS) -out:$@ $** $(ENTRY_FLAG) $(LIBS)

