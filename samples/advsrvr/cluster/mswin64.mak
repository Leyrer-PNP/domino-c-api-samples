#
# Makefile for HCL C API for Domino and Notes sample program, INTROWIN
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions

!include <win.mak>

proj = CLUMON

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	

##
.c.obj :
	$(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c
	
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
	$(link) $(linkdebug) $(guiflags) $(proj).obj clfunc.obj $(proj).res \
		$(guilibs) notes.lib -out:$(proj).exe
 
