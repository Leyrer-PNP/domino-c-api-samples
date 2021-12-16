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

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

#
outdllname = billing
outexename = billing
defname = mswin64

all : $(outdllname).DLL $(outexename).EXE

# BILLMNGR billing extension manager sample DLL
$(outdllname).DLL : billmngr.OBJ $(defname).DEF
    $(link) $(linkdebug) \
#        /NODEFAULTLIB \
	-dll -def:$(defname).DEF \
#	-entry:_DllMainCRTStartup$(DLLENTRY) \
	-out:$(outdllname).DLL \
	billmngr.OBJ \
	$(conlibsmt) notes.lib  msvcrt.lib

billmngr.OBJ : billmngr.C
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c

# BILLSES session billing sample addin
$(outexename).exe: billses.obj
	$(link) $(linkdebug)  $(conflags)  billses.obj notes0.obj notesai0.obj \
		$(conlibs) notes.lib user32.lib  libcmtd.lib -out:$(outexename).exe

billses.obj : billses.c
    !cl -nologo -c /ID:\notesapi90\include /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c



