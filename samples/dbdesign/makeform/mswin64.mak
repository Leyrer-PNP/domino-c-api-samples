#
# Makefile for Notes API sample program MAKEFORM 
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = makeform

# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ cdrecord.obj
$(PROGNAME).OBJ: $(PROGNAME).C $(PROGNAME).H
cdrecord.obj   : cdrecord.c $(PROGNAME).H

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W3 $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) kernel32.lib user32.lib notes.lib
