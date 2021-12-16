#
# makefile for Notes API sample program CDFREAD and CDFWRITE
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
#!include <ntwin32.mak>

#

# Master build target

all : cdfread.exe cdfwrite.exe

# Dependencies
!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
RUNTIME_FLAG =  libvcruntime.lib ucrt.lib libcmt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
RUNTIME_FLAG = msvcrt.lib
!ENDIF	


cdfread.exe : cdfread.obj

cdfread.obj : cdfread.c cdfile.h

cdfwrite.exe : cdfwrite.obj

cdfwrite.obj : cdfwrite.c cdfile.h

# Compilation command.

.C.OBJ:
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c

# Link command.

.OBJ.EXE:
    link -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpx  -debug -debugtype:cv -out:$@ $** notes.lib \
        $(RUNTIME_FLAG) kernel32.lib user32.lib gdi32.lib advapi32.lib winspool.lib ws2_32.lib userenv.lib
