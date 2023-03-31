#
# Makefile for HCL C API for Domino and Notes sample program, INTROWIN
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

proj = INTROWIN

all: $(proj).exe

# Update the resource file if necessary.
$(proj).res : $(proj).rc $(proj).h
        rc -r -fo $(proj).res -DWIN32 -D_WIN32 /DNT $(proj).rc

# Update the object file if necessary.
$(proj).obj : $(proj).c $(proj).h
    !cl $(COPTIONS) $*.c

# Update the executable file if necessary, and if so, add the resource
# back in.
$(proj).exe: $(proj).obj $(proj).res $(proj).def      
	link $(LOPTIONS) $(proj).obj $(proj).res \
        	-entry:WinMain ucrt.lib comdlg32.lib $(LIBS) mswsock.lib \
        	-out:$(proj).exe
