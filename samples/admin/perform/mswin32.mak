#
# Makefile for Notes API sample program ldap_msc
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.

#
# DEC Alpha doesn't want the structures to be packed so we use the CPU
# type to conditionally add it as part of compile string
#
!IF "$(CPU)" == "ALPHA"
cpuflags =
!ELSE
!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	
!ENDIF

PROGNAME1 = addnames
PROGNAME2 = index
PROGNAME3 = mixed_1
PROGNAME4 = mixed_2

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
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
    $(link) $(linkdebug) $(conflags) -out:$@ $** $(conlibs) \
        notes.lib user32.lib
