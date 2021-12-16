#
# makefile for Notes API sample program SECDOM.DLL
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

outfilename = secdom
defname = mswin32

all : $(outfilename).dll

$(outfilename).dll : $(outfilename).obj w_secdom.OBJ $(defname).def
    $(link) $(linkdebug) -base:0x1c000000 \
        -dll -def:$(defname).def \
         -out:$(outfilename).dll \
        $(outfilename).OBJ w_secdom.OBJ \
	$(conlibsdll) notes.lib mpr.lib	\
	/nodefaultlib:"libc"

$(outfilename).obj : $(outfilename).c
    $(cc) $(cdebug) $(cflags) $(cpuflags) -DNT  $(cvarsdll) $(outfilename).c

w_secdom.OBJ: w_secdom.c
	$(cc) $(cdebug) $(cflags) $(cpuflags) -DNT  $(cvarsdll) w_secdom.c

