#
# Makefile for HCL C API for Domino and Notes sample program, INTROWIN
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>


proj = CLUMON

	

##
.c.obj :
	!cl /DNT -Zi -Od -DDEBUG -c -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -nologo -GS -D_AMD64_=1 -DWIN64 -D_WIN64  -DWIN32 -D_WIN32 -W4 -D_WINNT -D_WIN32_WINNT=0x0500 -DNTDDI_VERSION=0x05000000 -D_WIN32_IE=0x0500 -DWINVER=0x0500  /DNT  -D_MT -MTd  $*.c
	
all: $(proj).exe

	
# Update the object file if necessary.
clumon.obj : clumon.c clumon.h
clfunc.obj : clfunc.c

# Update the resource file if necessary.
$(proj).res : $(proj).rc resource.h $(proj).ico
        rc -r -fo $(proj).res -DWIN64 -D_WIN64  /DNT $(proj).rc
		

# Update the executable file if necessary, and if so, add the resource
# back in.
$(proj).exe: $(proj).obj clfunc.obj  $(proj).res mswin64.def
	link $(LOPTIONS) $(proj).obj clfunc.obj $(proj).res \
		$(LIBS_1) mswsock.lib $(ENTRY_FLAG1) -out:$(proj).exe
 
