#
# makefile for Notes API sample program MGATE
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions

!include <makeEnvWin.mak>

##
!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
runtime =  libvcruntime.lib ucrt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
runtime = msvcrt.lib
!ENDIF

##
outfilename = nmgate

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename).exe: main.obj inbound.obj outbound.obj nmgate.res
	link /DEBUG /DEBUGTYPE:cv    main.obj inbound.obj outbound.obj notes0.obj notesai0.obj  nmgate.res kernel32.lib  ws2_32.lib mswsock.lib advapi32.lib msvcrt.lib oldnames.lib notes.lib user32.lib -out:$(outfilename).exe

# Update the resource file if necessary.
nmgate.res : mswin64.rc mgateerr.h
        rc -r -fo nmgate.res -DWIN32 -D_WIN32 /DNT mswin64.rc

# Update the object file if necessary.
main.obj : main.c mgate.h mgateerr.h
	!cl $(COPTIONS) main.c

inbound.obj : inbound.c mgate.h mgateerr.h
	!cl $(COPTIONS) inbound.c
	
outbound.obj : outbound.c mgate.h mgateerr.h
	!cl $(COPTIONS) outbound.c


