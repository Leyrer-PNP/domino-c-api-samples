# Makefile for Notes API sample program ldap_msc
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Standard Windows 64-bit make definitions
#!include <ntwin32.mak>
PROGNAME1 = addnames
PROGNAME2 = index
PROGNAME3 = mixed_1
PROGNAME4 = mixed_2
# The name of the program.

!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
ENTRY_FLAG =  -entry:mainCRTStartup libvcruntime.lib ucrt.lib libcmt.lib  
!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
ENTRY_FLAG = msvcrt.lib
!ENDIF	
##
ALL : $(PROGNAME1).EXE $(PROGNAME2).EXE $(PROGNAME3).EXE $(PROGNAME4).EXE 
# Dependencies

$(PROGNAME1).EXE: $(PROGNAME1).OBJ
$(PROGNAME1).OBJ: $(PROGNAME1).C
$(PROGNAME2).EXE: $(PROGNAME2).OBJ
$(PROGNAME2).OBJ: $(PROGNAME2).C
$(PROGNAME3).EXE: $(PROGNAME3).OBJ
$(PROGNAME3).OBJ: $(PROGNAME3).C
$(PROGNAME4).EXE: $(PROGNAME4).OBJ
$(PROGNAME4).OBJ: $(PROGNAME4).C

# Compilation command.
.C.OBJ:
    !cl -nologo   -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl  -DNT -DW32 -DW -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY $*.c

.OBJ.EXE:
    link -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpx  -debug -debugtype:cv -out:$@ $** notes.lib\
        $(ENTRY_FLAG) kernel32.lib user32.lib gdi32.lib advapi32.lib winspool.lib ws2_32.lib userenv.lib  

