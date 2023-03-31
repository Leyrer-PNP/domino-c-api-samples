#
# makefile for Notes API sample program LGIMPORT
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = lgimport

##

# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ CDRECORD.OBJ
$(PROGNAME).OBJ: $(PROGNAME).C
CDRECORD.OBJ: CDRECORD.C

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1) OLDNAMES.lib
