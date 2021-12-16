#
# Makefile for HCL C API for Domino and Notes sample program, INTROWIN
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions

!include <NTWIN32.MAK>

proj = INTROWIN

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
ENTRY_FLAG =  -entry:WinMain    ucrt.lib 
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
ENTRY_FLAG = libcmt.lib
!ENDIF	
##

all: $(proj).exe

# Update the resource file if necessary.
$(proj).res : $(proj).rc $(proj).h
        rc -r -fo $(proj).res -DWIN32 -D_WIN32 /DNT $(proj).rc

# Update the object file if necessary.
$(proj).obj : $(proj).c $(proj).h
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c

# Update the executable file if necessary, and if so, add the resource
# back in.
$(proj).exe: $(proj).obj $(proj).res $(proj).def      
	link $(linkdebug) $(guiflags) $(proj).obj $(proj).res \
            	-nologo -opt:ref  -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE  -debug -debugtype:cv \
        	$(ENTRY_FLAG) kernel32.lib user32.lib gdi32.lib advapi32.lib winspool.lib ws2_32.lib userenv.lib \
        	$(guilibs) notes.lib -out:$(proj).exe
