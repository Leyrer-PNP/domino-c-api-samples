#
# makefile for HCL C API for Domino and Notes sample program, UIADDIN1
# Windows 64-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the HCL C API and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>



# Master dependency

all : uiaddin1.dll


# Link command.

uiaddin1.dll : uiaddin1.obj mswin64.def
	link /DEBUG /DEBUGTYPE:cv -base:0x1C000000 -stack:0 \
    	-dll -entry:_DllMainCRTStartup$(DLLENTRY) \
	-def:mswin64.def \
    	-out:uiaddin1.dll \
    	uiaddin1.obj \
        $(LIBS_1) $(ENTRY_FLAG1) 


# Compilation command.

uiaddin1.obj: uiaddin1.c
	!cl /optimize -DNT $(COPTIONS) -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500 -D_MT -MTd uiaddin1.c
