#
# makefile for Notes API sample program EXTPWD.DLL
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#
outfilename = nextpwd
defname = mswin64

.c.obj :
    !cl $(COPTIONS) -W4 $*.c

all : $(outfilename).dll

$(outfilename).dll : extwin.obj extpwd.obj extwin.res $(defname).def
	link /DEBUG /DEBUGTYPE:cv \
        -dll -def:$(defname).def \
        -out:$(outfilename).dll \
        extwin.obj extpwd.obj extwin.res \
        $(LIBS_1) msvcrt.lib $(ENTRY_FLAG1) mswsock.lib comdlg32.lib

extpwd.obj : extpwd.h extpwdrs.h extpwd.c

extwin.obj : extpwd.h extpwdrs.h extwin.c

# Update the resource file if necessary.
extwin.res : extwin.rc extpwd.h
        rc -r -fo extwin.res -D_AMD64_ -DHANDLE_IS_32BITS -DNT -DW32 -DW -DW64 -DND64 -DPRODUCTION_VERSION -DDUMMY extwin.rc
