#
# makefile for Notes API sample program NOTESFLO
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the NotesAPI and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.

PROGNAME = notesflo

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

# File name macros

HEADERS = app_rsc.h ln_buff.hpp ln_flow.hpp nf_app.hpp nf_doc.hpp nf_hatch.hpp \
          nf_image.hpp nf_obj.hpp nf_store.hpp notesflo.h olecf.hpp oledo.hpp \
          oleec.hpp oleipao.hpp oleipo.hpp oleobj.hpp oleps.hpp

OBJECTS = notesflo.obj ln_flow.obj ln_buff.obj nf_app.obj nf_doc.obj nf_obj.obj \
          nf_store.obj nf_image.obj nf_hatch.obj nf_util.obj olecf.obj oledo.obj \
          oleec.obj oleipao.obj oleipo.obj oleobj.obj oleps.obj selcolor.obj

# Compilation command.

.CPP.OBJ:
    $(cc) $(cdebug) $(cflags) -GX $(cpuflags) /DW32 $(cvarsmt) $*.cpp

# Link command.

.OBJ.EXE:
    $(link) $(ldebug) $(guilflags) -out:$@ $** \
      $(olelibsmt) notes.lib libcmtd.lib

# Dependencies

all : notesflo.exe

notesflo.exe : $(OBJECTS) notesflo.res

notesflo.obj : notesflo.cpp $(HEADERS)

ln_flow.obj : ln_flow.cpp $(HEADERS)

ln_buff.obj : ln_buff.cpp $(HEADERS)

nf_app.obj : nf_app.cpp $(HEADERS)

nf_doc.obj : nf_doc.cpp $(HEADERS)

nf_obj.obj : nf_obj.cpp $(HEADERS)

nf_store.obj : nf_store.cpp $(HEADERS)

nf_image.obj : nf_image.cpp $(HEADERS)

nf_hatch.obj : nf_hatch.cpp $(HEADERS)

nf_util.obj : nf_util.cpp $(HEADERS)

olecf.obj : olecf.cpp $(HEADERS)

oledo.obj : oledo.cpp $(HEADERS)

oleec.obj : oleec.cpp $(HEADERS)

oleipao.obj : oleipao.cpp $(HEADERS)

oleipo.obj : oleipo.cpp $(HEADERS)

oleobj.obj : oleobj.cpp $(HEADERS)

oleps.obj : oleps.cpp $(HEADERS)

selcolor.obj : selcolor.cpp $(HEADERS)

# Resource compiler command

notesflo.res : app_rsc.rc app_rsc.h
    rc -r -fo notesflo.res $(rcvars) /DNT app_rsc.rc
