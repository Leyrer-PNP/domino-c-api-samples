#
# makefile for Notes API sample program FIND_DBS
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = find_dbs

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.
.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS) -out:$@ $** $(ENTRY_FLAG) $(LIBS)
