#
#  makefile for HCL C API for Domino and Notes sample program, DBDRIVE
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the HCL C API and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>


#
outfilename = ndbdbdrv
defname = mswin64

# Master dependency

all : $(outfilename).dll


# Link command.
	
$(outfilename).dll : dbdrive.obj mswin64.def
    $(link) $(linkdebug) -base:0x1C000000 -stack:0 \
	-dll \
#	-entry:_DllMainCRTStartup$(DLLENTRY) \
	-def:$(defname).def \
	-out:$(outfilename).dll \
	dbdrive.obj \
	$(conlibs) notes.lib msvcrt.lib OLDNAMES.lib


# Compilation command.

dbdrive.obj : dbdrive.c dbdrive.h
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c
