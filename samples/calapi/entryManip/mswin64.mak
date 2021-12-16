#
# Makefile for Notes API sample program entrymanip
# Windows 64-bit version using Microsoft Visual Studio 2017 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions

!include <ntwin32.mak>

# The name of the program.

PROGNAME = entrymanip

#cpuflags = /Zp

# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
    $(link) $(linkdebug) $(conflags) -out:$@ $** $(conlibs) \
        notes.lib user32.lib
