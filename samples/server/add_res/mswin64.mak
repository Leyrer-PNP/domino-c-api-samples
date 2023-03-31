#
# makefile for Notes API sample program ADD_RES
# Windows 64-bit version using
# Microsoft Visual Studio 2017
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>



##
outfilename = add_res

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename).exe: add_res.obj add_res.res
	link -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpn -debugtype:cv \
		add_res.obj $(NOTES_LIBS) \
		add_res.res $(LIBS_1) mswsock.lib $(ENTRY_FLAG1) -out:$(outfilename).exe

# Update the resource file if necessary.
add_res.res : add_res.rc add_res.h
	rc -r -fo add_res.res -DW32 -DW -DW64 -DND64 -DPRODUCTION_VERSION  -DDUMMY /DNT -D_AMD64_ -DHANDLE_IS_32BITS add_res.rc

# Update the object file if necessary.
add_res.obj : add_res.c add_res.h
	!cl $(COPTIONS) add_res.c
