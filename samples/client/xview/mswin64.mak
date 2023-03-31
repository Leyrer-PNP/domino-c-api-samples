#
# makefile for Notes API sample program XVIEW
# Windows 64-bit version using Microsoft Visual Studio 2010
# compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>


outfilename = nxview
defname = mswin64

# Master dependency

all : $(outfilename).dll

# Link command.

$(outfilename).dll : nxview.obj nxview.res $(defname).def
	link $(LOPTIONS) -base:0x1C000000 -stack:0 \
    	-dll -entry:_DllMainCRTStartup$(DLLENTRY) \
      -def:$(defname).def \
    	-out:$(outfilename).dll \
    	nxview.obj \
    	nxview.res \
	$(LIBS_1) $(ENTRY_FLAG1)

# Update the resource if necessary

nxview.res: mswin64.rc xview.h
    rc -v -r -fo nxview.res mswin64.rc

# Compilation command.

nxview.obj : xview.c xview.h
	!cl -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -D_WINNT -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500 /optimize -DW32 $(COPTIONS) -D_MT -MTd -Fonxview.obj xview.c
