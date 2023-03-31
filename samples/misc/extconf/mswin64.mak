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
outfilename = nextconf
defname = mswin64

.c.obj :
    !cl $(COPTIONS) -W4 $*.c

all : $(outfilename).dll

$(outfilename).dll : extcwin.obj extconf.obj $(defname).def
	link /DEBUG /DEBUGTYPE:cv   \
        -dll -def:$(defname).def \
        -out:$(outfilename).dll \
        extcwin.obj extconf.obj \
        $(LIBS_1) msvcrt.lib $(ENTRY_FLAG1) mswsock.lib comdlg32.lib

extconf.obj : extconf.h extconf.c

extcwin.obj : extcwin.c
