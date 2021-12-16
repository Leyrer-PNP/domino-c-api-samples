#
#  makefile for Notes API sample program EXTCONF
#  Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
RUNTIME_FLAG =  libvcruntime.lib ucrt.lib libcmt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
RUNTIME_FLAG = msvcrt.lib
!ENDIF	
#
outfilename = nextconf
defname = mswin64

all : $(outfilename).dll

.c.obj :
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c


$(outfilename).dll : extconf.obj extcwin.obj $(defname).def
    $(link) $(linkdebug) \
        -dll -def:$(defname).def \
#         -entry:_DllMainCRTStartup$(DLLENTRY) \
         -out:$(outfilename).dll \
        extconf.obj extcwin.obj \
        $(guilibs) notes.lib $(RUNTIME_FLAG)

extconf.obj : extconf.c

extcwin.obj : extcwin.c
