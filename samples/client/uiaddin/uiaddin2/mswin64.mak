#
# Makefile for HCL C API for Domino and Notes sample program, UIADDIN2
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the HCL C API and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#cpuflags = -Zp

# Master dependency

all : uiaddin2.dll

# Link command.

uiaddin2.dll : uiaddin2.obj mswin64.def
	link /DEBUG /DEBUGTYPE:cv -base:0x1C000000 -stack:0 \
    	-dll -entry:_DllMainCRTStartup$(DLLENTRY) -def:mswin64.def \
    	-out:uiaddin2.dll \
    	uiaddin2.obj \
	$(LIBS_1) $(ENTRY_FLAG1) 


# Compilation command.

uiaddin2.obj: uiaddin2.c
	!cl -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500 /optimize -DNT $(COPTIONS)  uiaddin2.c
