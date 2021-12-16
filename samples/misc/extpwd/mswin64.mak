#
# makefile for Notes API sample program EXTPWD.DLL
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

#
outfilename = nextpwd
defname = mswin64

.c.obj :
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c

all : $(outfilename).dll

$(outfilename).dll : extwin.obj extpwd.obj extwin.res $(defname).def
    $(link) $(linkdebug) \
        -dll -def:$(defname).def \
        -out:$(outfilename).dll \
        extwin.obj extpwd.obj extwin.res \
        $(guilibs) notes.lib msvcrt.lib

extpwd.obj : extpwd.h extpwdrs.h extpwd.c

extwin.obj : extpwd.h extpwdrs.h extwin.c

# Update the resource file if necessary.
extwin.res : extwin.rc extpwd.h
        rc -r -fo extwin.res -D_AMD64_ -DHANDLE_IS_32BITS -DNT -DW32 -DW -DW64 -DND64 -DPRODUCTION_VERSION -DDUMMY extwin.rc
