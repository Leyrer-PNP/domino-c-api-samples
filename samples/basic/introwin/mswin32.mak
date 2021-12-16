#
# Makefile for HCL C API for Domino and Notes sample program, INTROWIN
# Windows 32-bit version using Microsoft Visual Studio 2017 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.
!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)

!include <NTWIN32.MAK>

proj = INTROWIN

!IF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
cpuflags = /Zp
!ENDIF	


all: $(proj).exe

# Update the resource file if necessary.
$(proj).res : $(proj).rc $(proj).h
        rc -r -fo $(proj).res -DWIN32 -D_WIN32 /DNT $(proj).rc

# Update the object file if necessary.
$(proj).obj : $(proj).c $(proj).h
        $(cc) $(cdebug) $(cflags) $(cpuflags) /DNT $(cvars) $(include) $(proj).c

# Update the executable file if necessary, and if so, add the resource
# back in.
$(proj).exe: $(proj).obj $(proj).res $(proj).def
        $(link) $(linkdebug) $(guiflags) $(proj).obj $(proj).res \
                $(guilibs) notes.lib -out:$(proj).exe

!ELSE

!MESSAGE "Compiler version not supported"	
!MESSAGE "Need Compiler version 19.xx.xxxxx"
		
!ENDIF	
