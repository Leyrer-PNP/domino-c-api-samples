#
#  makefile for Notes API test program NEXTMNGR.DLL
#  Windows 32-bit version using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

outfilename = nextmngr
defname = mswin32

all : $(outfilename).DLL

$(outfilename).DLL : extmngr.OBJ createdb.OBJ cdrecord.OBJ $(defname).DEF
    $(link) $(linkdebug) \
        -dll -def:$(defname).DEF \
         -entry:_DllMainCRTStartup$(DLLENTRY) \
         -out:$(outfilename).DLL \
        extmngr.OBJ createdb.OBJ cdrecord.OBJ \
        $(guilibs) notes.lib


extmngr.OBJ : extmngr.H extmngr.C
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) extmngr.C

createdb.OBJ : createdb.C
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) createdb.C

cdrecord.OBJ : cdrecord.C
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) cdrecord.C
