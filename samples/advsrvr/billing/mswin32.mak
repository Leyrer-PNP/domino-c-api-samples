#
# makefile for Notes API sample program BILLING.
#		Windows 32-bit version
#
# This makefile is designed for use under Windows 32-bit
# using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows NT make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF   

outdllname = nbillmgr
outexename = nbillses
defname = mswin32

all : $(outdllname).DLL $(outexename).EXE

# BILLMNGR billing extension manager sample DLL
$(outdllname).DLL : billmngr.OBJ $(defname).DEF
    $(link) $(linkdebug) \
#        /NODEFAULTLIB \
	-dll -def:$(defname).DEF \
#	-entry:_DllMainCRTStartup$(DLLENTRY) \
	-out:$(outdllname).DLL \
	billmngr.OBJ \
	$(conlibsmt) notes.lib

billmngr.OBJ : billmngr.C
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) billmngr.C

# BILLSES session billing sample addin
$(outexename).exe: billses.obj
	$(link) $(linkdebug) $(conflags) billses.obj notes0.obj notesai0.obj \
		$(conlibs) notes.lib user32.lib -out:$(outexename).exe

billses.obj : billses.c
	$(cc) $(cdebug) $(cflags) $(cvars) $(cpuflags) /ID:\notesapi90\include -DNT billses.c


