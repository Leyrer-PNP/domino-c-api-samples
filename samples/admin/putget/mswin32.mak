#
# makefile for Notes API sample program PUTGET
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>


!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	

ALL: executeputnote.exe getnote.exe


executeputnote.EXE: put_ui.OBJ executeputnote.OBJ fields.OBJ file_io.OBJ misc.OBJ
getnote.EXE: get_ui.OBJ getnote.OBJ fields.OBJ file_io.OBJ misc.OBJ

get_ui.OBJ: get_ui.C constant.h putget.h
put_ui.OBJ: put_ui.C constant.h putget.h
executeputnote.OBJ: executeputnote.C constant.h file_io.h misc.h fields.h
getnote.OBJ: getnote.C constant.h file_io.h misc.h fields.h
fields.OBJ: fields.C constant.h file_io.h misc.h
file_io.OBJ: file_io.C constant.h
misc.OBJ: misc.C constant.h

# Compilation command.

.C.OBJ:
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c

# Link command.

.OBJ.EXE:
    $(link) $(linkdebug) $(conflags) -out:$@ $** $(conlibs) \
    	notes.lib user32.lib
