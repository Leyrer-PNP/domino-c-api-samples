#
# makefile for Notes API sample program idbackup
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>



outfilename = idbackup

# Update the executable file if necessary
$(outfilename).exe: idbackup.obj
	link $(LOPTIONS_1) -out:$@ $** $(ENTRY_FLAG1) $(LIBS_1) $(NOTES_LIBS)


idbackup.obj : idbackup.c
	!cl $(COPTIONS) -W4 $*.c



