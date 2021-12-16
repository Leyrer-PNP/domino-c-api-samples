#
# Makefile for Notes API sample program MAKEFORM Windows 32-bit
# using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = makeform

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF   

# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ cdrecord.obj
$(PROGNAME).OBJ: $(PROGNAME).C $(PROGNAME).H
cdrecord.obj   : cdrecord.c $(PROGNAME).H

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
   $(link) $(linkdebug) $(conflags) -out:$@ $** $(conlibs) \
       notes.lib user32.lib

