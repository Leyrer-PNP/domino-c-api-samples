#
# makefile for Notes API sample program DUSSPI
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	
outfilename = ndusspi
defname = mswin32


# Master dependency

all : $(outfilename).dll

# Link command.
	
$(outfilename).DLL : dusspi.obj dusspi.res $(defname).def
    $(link) $(linkdebug) -base:0x1C000000 \
	-dll -entry:_DllMainCRTStartup$(DLLENTRY) \
        -def:$(defname).def \
	-out:$(outfilename).DLL \
	dusspi.obj \
	dusspi.res \
		$(conlibs) $(guilibs) notes.lib

# Update the resource if necessary

dusspi.res: mswin32.rc duspierr.h
    rc -v -r -fo dusspi.res mswin32.rc

# Compilation command.

dusspi.obj : dusspi.c dusspi.h duspierr.h
        $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) dusspi.c
