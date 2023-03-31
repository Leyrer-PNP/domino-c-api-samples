#
# makefile for Notes API sample program PUTGET.
#
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#Target
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
    !cl $(COPTIONS)  $*.c

# Link command.

.OBJ.EXE:
    link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG) $(LIBS_1)
