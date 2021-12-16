#
#  makefile for HCL C API for Domino and Notes sample program, DBDRIVE
#  Windows 32-bit version using Microsoft Visual Studio 2010
#  compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the HCL C API and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>


!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

outfilename = ndbdbdrv
defname = mswin32

# Master dependency

all : $(outfilename).dll


# Link command.
	
$(outfilename).dll : dbdrive.obj mswin32.def
    $(link) $(linkdebug) -base:0x1C000000 -stack:0 \
	-dll -entry:_DllMainCRTStartup$(DLLENTRY) -def:$(defname).def \
	-out:$(outfilename).dll \
	dbdrive.obj \
	$(conlibs) notes.lib


# Compilation command.

dbdrive.obj : dbdrive.c dbdrive.h
        $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) dbdrive.c
