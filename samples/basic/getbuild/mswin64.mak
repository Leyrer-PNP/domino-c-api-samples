#
# Makefile for Notes API sample program getbuild
# using Microsoft Visual Studio 2017
# and linker.
#

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = GETBUILD

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.
.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG) $(LIBS_1)
