#
#  makefile for Notes API sample program EXTCONF
#  Windows 32-bit version using Microsoft Visual Studio 2010
#  compiler and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

outfilename = nextconf
defname = mswin32

all : $(outfilename).dll

.c.obj :
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) $*.c


$(outfilename).dll : extconf.obj extcwin.obj $(defname).def
    $(link) $(linkdebug) \
        -dll -def:$(defname).def \
         -entry:_DllMainCRTStartup$(DLLENTRY) \
         -out:$(outfilename).dll \
        extconf.obj extcwin.obj \
        $(guilibs) notes.lib

extconf.obj : extconf.c

extcwin.obj : extcwin.c
