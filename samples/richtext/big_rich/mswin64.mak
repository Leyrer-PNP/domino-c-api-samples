#
# makefile for Notes API sample program BIG_RICH
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

##

# Targets.

ALL: BIG_FILE.EXE BIG_RICH.EXE

# Dependencies.

BIG_FILE.EXE: BIG_FILE.OBJ
	$(link) $(linkdebug) $(conflags) -out:$@ big_file.obj $(conlibs) \
    		user32.lib  msvcrt.lib OLDNAMES.lib


BIG_FILE.OBJ: BIG_FILE.C
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c



BIG_RICH.EXE: BIG_RICH.OBJ
	$(link) $(linkdebug) $(conflags) -out:$@ big_rich.obj $(conlibs) \
    		notes.lib user32.lib msvcrt.lib OLDNAMES.lib



BIG_RICH.OBJ: BIG_RICH.C
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c
