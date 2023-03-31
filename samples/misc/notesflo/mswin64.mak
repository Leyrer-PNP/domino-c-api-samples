#
# makefile for Notes API sample program DUSSPI
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

all : notesflo.exe

HEADERS = app_rsc.h ln_buff.hpp ln_flow.hpp nf_app.hpp nf_doc.hpp nf_hatch.hpp \
          nf_image.hpp nf_obj.hpp nf_store.hpp notesflo.h olecf.hpp oledo.hpp \
          oleec.hpp oleipao.hpp oleipo.hpp oleobj.hpp oleps.hpp

OBJECTS = notesflo.obj ln_flow.obj ln_buff.obj nf_app.obj nf_doc.obj nf_obj.obj \
          nf_store.obj nf_image.obj nf_hatch.obj nf_util.obj olecf.obj oledo.obj \
          oleec.obj oleipao.obj oleipo.obj oleobj.obj oleps.obj selcolor.obj


notesflo.exe : $(OBJECTS) notesflo.res

notesflo.obj : notesflo.cpp  $(HEADERS)

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

# Compilation command.

.CPP.OBJ:
	!cl $(COPTIONS)  $*.cpp

# Link command.

.OBJ.EXE:
	link -nologo -opt:ref -machine:AMD64 -map:w64_cmp.mpx -debug -debugtype:cv -out:$@ $** notes.lib \
	msvcrt.lib kernel32.lib user32.lib gdi32.lib advapi32.lib ws2_32.lib userenv.lib uuid.lib ole32.lib comdlg32.lib

# Update the resource if necessary
notesflo.res : app_rsc.rc app_rsc.h
	rc -r -fo notesflo.res  app_rsc.rc
