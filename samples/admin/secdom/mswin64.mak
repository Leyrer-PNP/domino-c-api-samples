#
# makefile for Notes API sample program SECDOM.DLL
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
#!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
runtime =  libvcruntime.lib ucrt.lib libcmt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
runtime = msvcrt.lib
!ENDIF	
           
#
outfilename = secdom
defname = mswin64

all : nsecdom.dll

nsecdom.dll : $(outfilename).obj w_secdom.OBJ $(defname).def
    link -nologo -opt:ref -pdb:$(outfilename).pdb -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -subsystem:windows \
        -dll -def:$(defname).def -entry:MainEntryPoint -out:nsecdom.dll secdom.OBJ w_secdom.OBJ kernel32.lib user32.lib \
        gdi32.lib advapi32.lib comctl32.lib comdlg32.lib notes.lib $(runtime) \
        kernel32.lib user32.lib gdi32.lib winspool.lib 

$(outfilename).obj : $(outfilename).c
        !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY -Fosecdom.obj -DSEG_secdom secdom.c

w_secdom.OBJ: w_secdom.c
	!cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY -Fow_secdom.obj -DSEG_secdom w_secdom.c

