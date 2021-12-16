#
# makefile for Notes API sample program THREADS
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

##
outfilename = threads

# Update the executable file if necessary
$(outfilename).exe: threads.obj
	$(link) $(linkdebug) $(conflags) threads.obj notes0.obj notesai0.obj \
		$(conlibsmt) msvcrt.lib oldnames.lib notes.lib user32.lib -out:$(outfilename).exe

# Update the object file if necessary.
threads.obj : threads.c
	!cl -nologo -c /Id:\notesapi90\include /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY threads.c
