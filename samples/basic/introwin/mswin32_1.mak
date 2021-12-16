#
# Makefile for HCL C API for Domino and Notes sample program, INTROWIN
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

#!include <NTWIN32.MAK>

proj = INTROWIN

cpuflags = /Zp

include = ..\..\..\include\global.h "C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include\Windows.h"

all: $(proj).exe

# Update the resource file if necessary.
#$(proj).res : $(proj).rc $(proj).h
#        rc -r -fo $(proj).res -DWIN32 -D_WIN32  $(proj).rc

# Update the object file if necessary.
$(proj).obj : $(proj).c $(proj).h 
        $(cc) $(cdebug) $(cflags)  $(cvars) $(include) $(proj).c

# Update the executable file if necessary, and if so, add the resource
# back in.
$(proj).exe: $(proj).obj $(proj).res $(proj).def
        $(link) $(linkdebug) $(guiflags) $(proj).obj $(proj).res \
                $(guilibs) notes.lib -out:$(proj).exe
