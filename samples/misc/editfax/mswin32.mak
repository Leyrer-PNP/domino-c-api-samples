#
# makefile for Notes API sample program intro.
#		Windows 32-bit version
#
# This makefile is designed for use under Windows 32-bit
#	using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows NT make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = editfax

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

clean:
	rm *.obj,*.exe

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
    $(link) $(linkdebug) $(conflags) -out:$@ $** $(conlibs) $(guilibs) \
    	notes.lib editfax.lib user32.lib
