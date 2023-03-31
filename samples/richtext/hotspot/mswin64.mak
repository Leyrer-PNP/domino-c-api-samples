#
# makefile for Notes API sample program HOTSPOT
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = hotspot



$(PROGNAME).EXE: $(PROGNAME).OBJ cdrecord.obj
$(PROGNAME).OBJ: $(PROGNAME).C cdrecord.c

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W4 $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
