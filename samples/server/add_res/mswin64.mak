#
# makefile for Notes API sample program ADD_RES
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
!include <ntwin32.mak>

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
RUNTIME_FLAG =  libvcruntime.lib ucrt.lib libcmt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
RUNTIME_FLAG = msvcrt.lib
!ENDIF

##
outfilename = add_res

# Update the executable file if necessary, and if so, add the resource
# back in.
$(outfilename).exe: add_res.obj add_res.res
	$(link) -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpn  -debugtype:cv \
		add_res.obj notes0.obj notesai0.obj \
		add_res.res $(conlibs)  $(RUNTIME_FLAG) notes.lib user32.lib -out:$(outfilename).exe

# Update the resource file if necessary.
add_res.res : add_res.rc add_res.h
	rc -r -fo add_res.res -DW32 -DW -DW64 -DND64 -DPRODUCTION_VERSION  -DDUMMY /DNT -D_AMD64_ -DHANDLE_IS_32BITS add_res.rc

# Update the object file if necessary.
add_res.obj : add_res.c add_res.h
	!cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -W4 -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY add_res.c
