#
# makefile for Notes API sample program NSF_DUMP.
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.

PROGNAME = USER_DEF

	

all:  c_usrdef.exe r_usrdef.exe

# Dependencies

c_usrdef.exe : c_usrdef.obj proc_arg.obj

c_usrdef.obj : c_usrdef.c proc_arg.c

r_usrdef.exe : r_usrdef.obj proc_arg.obj

r_usrdef.obj : r_usrdef.c proc_arg.c



# Compilation command.

.C.OBJ:
    !cl $(COPTIONS) -W4 $*.c

# Link command.
	
.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1)
