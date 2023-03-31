#
# makefile for Notes API sample program THREADS
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

##
outfilename = threads

# Update the executable file if necessary
$(outfilename).exe: threads.obj
	link /DEBUG /DEBUGTYPE:cv  /INCREMENTAL:NO /NOLOGO -subsystem:console,5.0 threads.obj $(NOTES_LIBS)\
		$(LIBS) mswsock.lib -out:$(outfilename).exe 

# Update the object file if necessary.
threads.obj : threads.c
	!cl $(COPTIONS)  threads.c
