#
# Makefile for Notes API sample program MSIMPLE
# Windows 32-bit version using Microsoft Visual Studio 2017 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.
!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = msimple


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

!ELSE

!MESSAGE "Compiler version not supported"	
!MESSAGE "Need Compiler version 19.xx.xxxxx"
		
!ENDIF	