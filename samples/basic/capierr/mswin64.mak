#
# Makefile for Notes API sample program CapiErr.
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

# The name of the program.
PROGNAME = capierr

#

# Dependencies
$(PROGNAME).exe: $(PROGNAME).obj $(PROGNAME).res $(PROGNAME)D.obj StdAfx.obj
$(PROGNAME).obj		: $(PROGNAME).cpp
$(PROGNAME).res		: $(PROGNAME).rc
	rc -fo $(PROGNAME).res /DNT /d_AFXDLL $(PROGNAME).rc
$(PROGNAME)Dlg.obj		: $(PROGNAME)D.cpp
StdAfx.obj		: StdAfx.cpp



# Compilation command.
.cpp.obj:
	!cl $(COPTIONS1) $*.cpp

# Link command.
.obj.exe:
    link -nologo -opt:ref -machine:AMD64  /LARGEADDRESSAWARE /subsystem:windows /verbose -map:w64_cmp.mpx  -debug -debugtype:cv -out:$@ $** notes.lib \
        kernel32.lib user32.lib gdi32.lib advapi32.lib winspool.lib ws2_32.lib userenv.lib
