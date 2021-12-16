#
# Makefile for Notes API sample program nsfquery.
# Windows 64-bit version using
# Microsoft Visual Studio 2017 SP1.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions.
!include <ntwin32.mak>

#

# The name of the program.

PROGNAME = nsfquery

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
ENTRY_FLAG =  libvcruntime.lib ucrt.lib libcmt.lib
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
ENTRY_FLAG = msvcrt.lib
!ENDIF

# Dependencies.

$(PROGNAME).EXE: $(PROGNAME).OBJ
$(PROGNAME).OBJ: $(PROGNAME).C

# Compilation command.
.C.OBJ:
    !cl -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c

# Link command.

.OBJ.EXE:
    link -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpx  -debug -debugtype:cv -out:$@ $** notes.lib \
        $(ENTRY_FLAG) kernel32.lib
