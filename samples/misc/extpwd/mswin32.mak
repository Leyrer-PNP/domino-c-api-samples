#
# makefile for Notes API sample program EXTPWD.DLL
# Windows 32-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF

outfilename = nextpwd
defname = mswin32

.c.obj :
    $(cc) $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) $*.c

all : $(outfilename).dll

$(outfilename).dll : extwin.obj extpwd.obj extwin.res $(defname).def
    $(link) $(linkdebug) -base:0x1c000000 \
        -dll -def:$(defname).def \
        -out:$(outfilename).dll \
        extwin.obj extpwd.obj extwin.res \
        $(guilibs) notes.lib

extpwd.obj : extpwd.h extpwdrs.h extpwd.c

extwin.obj : extpwd.h extpwdrs.h extwin.c

# Update the resource file if necessary.
extwin.res : extwin.rc extpwd.h
        rc -r -fo extwin.res -DWIN32 -D_WIN32 /DNT extwin.rc
