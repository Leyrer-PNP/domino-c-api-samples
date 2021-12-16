#
# makefile for Notes API sample program EVENTS
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

PROGNAME1 = consume
PROGNAME2 = produce
outfilename1 = $(PROGNAME1)
outfilename2 = $(PROGNAME2)

##

all: $(outfilename1).exe $(outfilename2).exe

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename1).exe: $(PROGNAME1).obj $(PROGNAME1).res
	$(link) $(linkdebug)  $(conflags)  $(PROGNAME1).obj notes0.obj notesai0.obj \
	$(PROGNAME1).res $(conlibs) msvcrt.lib OLDNAMES.lib notes.lib user32.lib -out:$(outfilename1).exe

# Update the resource file if necessary.
$(PROGNAME1).res : $(PROGNAME1).rc $(PROGNAME1).h
        rc -r -fo $(PROGNAME1).res -DWIN32 -D_WIN32 /DNT $(PROGNAME1).rc

# Update the object file if necessary.
$(PROGNAME1).obj : $(PROGNAME1).c $(PROGNAME1).h
	!cl -nologo -c /Id:\notesapi90\include /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY  $(PROGNAME1).c

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename2).exe: $(PROGNAME2).obj $(PROGNAME2).res
	$(link) $(linkdebug)  $(conflags)  $(PROGNAME2).obj notes0.obj notesai0.obj \
	$(PROGNAME2).res $(conlibs) msvcrt.lib OLDNAMES.lib notes.lib user32.lib -out:$(outfilename2).exe

# Update the resource file if necessary.
$(PROGNAME2).res : $(PROGNAME2).rc $(PROGNAME2).h
        rc -r -fo $(PROGNAME2).res -DWIN32 -D_WIN32 /DNT $(PROGNAME2).rc

# Update the object file if necessary.
$(PROGNAME2).obj : $(PROGNAME2).c $(PROGNAME2).h
	!cl -nologo -c /Id:\notesapi90\include /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $(PROGNAME2).c

