# Makefile for Notes API sample program perform
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

PROGNAME1 = addnames
PROGNAME2 = index
PROGNAME3 = mixed_1
PROGNAME4 = mixed_2
# The name of the program.

ALL : $(PROGNAME1).EXE $(PROGNAME2).EXE $(PROGNAME3).EXE $(PROGNAME4).EXE 
# Dependencies

$(PROGNAME1).EXE: $(PROGNAME1).OBJ
$(PROGNAME1).OBJ: $(PROGNAME1).C
$(PROGNAME2).EXE: $(PROGNAME2).OBJ
$(PROGNAME2).OBJ: $(PROGNAME2).C
$(PROGNAME3).EXE: $(PROGNAME3).OBJ
$(PROGNAME3).OBJ: $(PROGNAME3).C
$(PROGNAME4).EXE: $(PROGNAME4).OBJ
$(PROGNAME4).OBJ: $(PROGNAME4).C

# Compilation command.
.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG) $(LIBS_1)
