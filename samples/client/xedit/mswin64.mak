#
# makefile for Notes API sample program XEDIT
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>


outfilename = nxedit
defname = mswin64

# Master dependency

all : $(outfilename).dll

# Link command.

$(outfilename).dll : nxedit.obj nxedit.res $(defname).def
	link /DEBUG /DEBUGTYPE:cv -base:0x1C000000 -stack:0 \
    	-dll -entry:_DllMainCRTStartup$(DLLENTRY) \
      -def:$(defname).def \
    	-out:$(outfilename).dll \
    	nxedit.obj \
    	nxedit.res mswsock.lib comdlg32.lib \
	$(LIBS_1) msvcrt.lib OLDNAMES.lib

# Update the resource if necessary

nxedit.res: mswin64.rc xedit.h
    rc -v -r -fo nxedit.res mswin64.rc

# Compilation command.

nxedit.obj : xedit.c xedit.h
	!cl -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -D_MT -MTd -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500 /optimize -DNT $(COPTIONS) -DW32 -Fonxedit.obj xedit.c
