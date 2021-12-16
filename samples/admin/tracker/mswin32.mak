#
#  makefile for Notes API test program NTRACKER.DLL
#  Windows 32-bit version using Microsoft Visual Studio 2010 compiler
#  and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>


!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	
outfilename = ntracker
defname = mswin32

all : $(outfilename).DLL reqnum.exe

$(outfilename).DLL : tracker.OBJ $(defname).DEF
    $(link) $(linkdebug) \
        -dll -def:$(defname).DEF \
         -entry:_DllMainCRTStartup$(DLLENTRY) \
         -out:$(outfilename).DLL \
        tracker.OBJ \
        $(guilibs) notes.lib


tracker.OBJ : tracker.H
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) tracker.C


reqnum.exe : reqnum.obj
   $(link) $(linkdebug) $(conflags) -out:reqnum.exe reqnum.obj $(conlibs) \
        notes.lib user32.lib

reqnum.obj : reqnum.c
    $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $*.c
