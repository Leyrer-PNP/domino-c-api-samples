#
# makefile for Notes API sample program NSF_DUMP.
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Standard Windows NT make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = USER_DEF

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

all: r_usrdef.exe c_usrdef.exe

# Dependencies

r_usrdef.exe : r_usrdef.obj proc_arg.obj

c_usrdef.exe : c_usrdef.obj proc_arg.obj

r_usrdef.obj : r_usrdef.c proc_arg.c

c_usrdef.obj : c_usrdef.c proc_arg.c

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.
	
.OBJ.EXE:
    $(link) $(linkdebug) $(conflags) -out:$@ $** \
      $(conlibs) notes.lib user32.lib