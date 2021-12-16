#
#  makefile for Notes API test program NEXTMAIL.DLL
#  Windows 32-bit version using Microsoft Visual Studio 2010 compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF 

outfilename = extmail
defname = mswin32

all : $(outfilename).DLL

$(outfilename).DLL : extmail.OBJ $(defname).DEF
    $(link) $(linkdebug) \
        -dll -def:$(defname).DEF \
         -entry:_DllMainCRTStartup$(DLLENTRY) \
         -out:$(outfilename).DLL \
        extmail.OBJ \
        $(guilibs) notes.lib


extmail.OBJ : extmail.h extmail.C 
    $(cc)  $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) extmail.C
    