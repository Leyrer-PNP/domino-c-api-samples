#
# Makefile for Notes API sample program CapiErr.
# Windows 32-bit version using Microsoft Visual Studio 2017 compiler
# and linker.
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.
!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

# The name of the program.
PROGNAME = CapiErr

cpuflags = -Zp

# Dependencies
$(PROGNAME).exe: $(PROGNAME).obj $(PROGNAME).res $(PROGNAME)D.obj StdAfx.obj
$(PROGNAME).obj		: $(PROGNAME).cpp
$(PROGNAME).res		: $(PROGNAME).rc
	rc -fo $(PROGNAME).res /DNT /d_AFXDLL $(PROGNAME).rc
$(PROGNAME)Dlg.obj		: $(PROGNAME)D.cpp
StdAfx.obj		: StdAfx.cpp



# Compilation command.
.cpp.obj:
#   $(cc) $(cdebug) -GX -MD /optimize -D_MBCS -FD -DW32 -DNODLL -D_AFXDLL $(cpuflags) $*.cpp
   $(cc) -Z7 -Od -c -W3 -DCRTAPI1=_cdecl -DCRTAPI2=_cdecl -nologo \
  	-D_X86_=1 -D_WINNT -D_WIN32_WINNT=0x0501 -D_WIN32_IE=0x0501 -DWINVER=0x501 \
		-GX -MD /optimize -D_MBCS -FD -DW32 -DNODLL -D_AFXDLL -Zpl $*.cpp

# Link command.
.obj.exe:
   $(link) $(linkdebug) $(DEF_FILE) kernel32.lib user32.lib gdi32.lib winspool.lib \
   	comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib \
   	odbccp32.lib /nologo notes.lib /subsystem:windows /incremental:no /machine:I386 \
    	-subsystem:windows -out:$@ $**

!ELSE

!MESSAGE "Compiler version not supported"	
!MESSAGE "Need Compiler version 19.xx.xxxxx"
		
!ENDIF	

