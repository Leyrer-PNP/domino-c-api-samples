#
#  makefile for HCL C API for Domino and Notes sample program, DBDRIVE
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the HCL C API and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>


#
outfilename = ndbdbdrv
defname = mswin64

# Master dependency

all : $(outfilename).dll


# Link command.
	
$(outfilename).dll : dbdrive.obj mswin64.def
	link /DEBUG /DEBUGTYPE:cv -base:0x1C000000 -stack:0 \
	-dll \
#	-entry:_DllMainCRTStartup$(DLLENTRY) \
	-def:$(defname).def \
	-out:$(outfilename).dll \
	dbdrive.obj \
	$(LIBS) mswsock.lib 


# Compilation command.

dbdrive.obj : dbdrive.c dbdrive.h
    !cl $(COPTIONS) $*.c

