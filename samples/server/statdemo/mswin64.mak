#
# makefile for Notes API sample program STATDEMO
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

##
outfilename = statdemo

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename).exe: statdemo.obj statdemo.res
	$(link) $(linkdebug)  $(conflags) statdemo.obj notes0.obj notesai0.obj \
		statdemo.res $(conlibs) msvcrt.lib OLDNAMES.lib notes.lib user32.lib -out:$(outfilename).exe

# Update the resource file if necessary.
statdemo.res : statdemo.rc statdemo.h
        rc -r -fo statdemo.res -DWIN32 -D_WIN32 /DNT statdemo.rc

# Update the object file if necessary.
statdemo.obj : statdemo.c statdemo.h
	!cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY statdemo.c
