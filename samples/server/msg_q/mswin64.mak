#
# makefile for Notes API sample program MSG_Q
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

##
outfilename1 = msg_q
outfilename2 = msg_t

# targets
all: $(outfilename1).exe $(outfilename2).exe

# Update the executable file if necessary
$(outfilename1).exe: msg_q.obj
	$(link) $(linkdebug)  $(conflags)  msg_q.obj notes0.obj notesai0.obj \
		$(conlibs) msvcrt.lib OLDNAMES.lib notes.lib user32.lib -out:$(outfilename1).exe

# Update the object file if necessary.
msg_q.obj : msg_q.c
	!cl -nologo -c /Id:\notesapi90\include /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY  msg_q.c

# Update the executable file if necessary
$(outfilename2).exe: msg_t.obj
	$(link) $(linkdebug)  $(conflags)  msg_t.obj notes0.obj notesai0.obj \
		$(conlibs) msvcrt.lib OLDNAMES.lib notes.lib user32.lib -out:$(outfilename2).exe

# Update the object file if necessary.
msg_t.obj : msg_t.c
	!cl -nologo -c /Id:\notesapi90\include /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY  msg_t.c
