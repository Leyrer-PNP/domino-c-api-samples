#
# Makefile for HCL C API for Domino and Notes sample program, UIADDIN2
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the HCL C API and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF   

# Master dependency

all : uiaddin2.dll

# Link command.

uiaddin2.dll : uiaddin2.obj mswin32.def
    $(link) $(linkdebug) -base:0x1C000000 \
    	-dll -entry:_DllMainCRTStartup$(DLLENTRY) -def:mswin32.def \
    	-out:uiaddin2.dll \
    	uiaddin2.obj \
                $(guilibs) notes.lib


# Compilation command.

uiaddin2.obj: uiaddin2.c
        $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) uiaddin2.c
