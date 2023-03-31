#
#  makefile for Notes API sample program IEDIT
#  Windows 64-bit version using Microsoft Visual Studio 2010
#  compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>


outfilename = niedit
defname = mswin64

# Master dependency

all : $(outfilename).dll


# Link command.

$(outfilename).dll : iedit.obj ieditwin.obj $(defname).def
	link /DEBUG /DEBUGTYPE:cv -base:0x1C000000 -stack:0 \
    -dll -entry:_DllMainCRTStartup$(DLLENTRY) \
    -def:$(defname).def \
    -out:$(outfilename).dll \
    iedit.obj \
    ieditwin.obj \
    $(LIBS_1) mswsock.lib comdlg32.lib $(ENTRY_FLAG1)

# Compilation command.

iedit.obj : iedit.c iedit.h
	!cl /optimize -DNT -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -D_WINNT -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500  $(COPTIONS) iedit.c

ieditwin.obj : ieditwin.c iedit.h
	!cl /optimize -DNT -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -D_WINNT -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500  $(COPTIONS) ieditwin.c

