#
# Makefile for Notes API sample program runmacro
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = RUNMACRO

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
RUNTIME_FLAG =  libvcruntime.lib ucrt.lib libcmt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
RUNTIME_FLAG = msvcrt.lib
!ENDIF	


# Dependencies

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C $(PROGNAME).H

# Compilation command.

.C.OBJ:
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS) -out:$@ $** $(ENTRY_FLAG) $(LIBS)
