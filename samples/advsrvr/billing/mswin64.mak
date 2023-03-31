#
# makefile for Notes API sample program BILLING.
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.
#
# makefile for Notes API sample program BILLING.
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#
outdllname = billing
outexename = billing
defname = mswin64

all : $(outdllname).DLL $(outexename).EXE

# BILLMNGR billing extension manager sample DLL
$(outdllname).DLL : billmngr.OBJ $(defname).DEF
	link /DEBUG /DEBUGTYPE:cv  \
#        /NODEFAULTLIB \
	-dll -def:$(defname).DEF \
#	-entry:_DllMainCRTStartup$(DLLENTRY) \
	-out:$(outdllname).DLL \
	billmngr.OBJ mswsock.lib \
	$(LIBS) $(ENTRY_FLAG1)

billmngr.OBJ : billmngr.C
    !cl $(COPTIONS) $*.c

# BILLSES session billing sample addin
$(outexename).exe: billses.obj
	link $(LOPTIONS_1) -subsystem:console,5.0 billses.obj $(NOTES_LIBS) \
		$(LIBS_1)  $(ENTRY_FLAG1) libcmtd.lib mswsock.lib -out:$(outexename).exe

billses.obj : billses.c
	!cl $(COPTIONS) $*.c


