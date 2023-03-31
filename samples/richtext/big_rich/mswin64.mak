#
# makefile for Notes API sample program BIG_RICH
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>
##

# Targets.

ALL: BIG_FILE.EXE BIG_RICH.EXE

# Dependencies.

BIG_FILE.EXE: BIG_FILE.OBJ
	link $(LOPTIONS_1) -subsystem:console,5.0 -out:$@ big_file.obj $(LIBS) $(ENTRY_FLAG1) 


BIG_FILE.OBJ: BIG_FILE.C
    !cl $(COPTIONS) -W4 $*.c



BIG_RICH.EXE: BIG_RICH.OBJ
	link $(LOPTIONS_1) -subsystem:console,5.0 -out:$@ big_rich.obj $(LIBS) $(ENTRY_FLAG1) 



BIG_RICH.OBJ: BIG_RICH.C
    !cl $(COPTIONS) -W4 $*.c
