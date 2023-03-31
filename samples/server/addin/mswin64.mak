#
# makefile for Notes API sample program ADDIN
# Windows 64-bit version using Microsoft Visual Studio 2010 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

outfilename = addin

# Update the executable file if necessary
$(outfilename).exe: addin.obj
    link -nologo -opt:ref -machine:AMD64 /LARGEADDRESSAWARE -map:w64_cmp.mpx  -debug -debugtype:cv -out:$@ $** $(LIBS_1) msvcrt.lib $(NOTES_LIBS) 

# Update the object file if necessary.
addin.obj : addin.c
    !cl $(COPTIONS) $*.c
