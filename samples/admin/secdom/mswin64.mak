#
# makefile for Notes API sample program SECDOM.DLL
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

#!IF ([cl /? 2>&1 | findstr /C:"Version 19" > nul] == 0)
#runtime =  libvcruntime.lib ucrt.lib libcmt.lib  
#!ELSEIF ([cl /? 2>&1 | findstr /C:"Version 16" > nul] == 0)
#runtime = msvcrt.lib
#!ENDIF	
           
#
outfilename = secdom
defname = mswin64

all : nsecdom.dll

nsecdom.dll : $(outfilename).obj w_secdom.OBJ $(defname).def
    link $(LOPTIONS) -pdb:$(outfilename).pdb -dll -def:$(defname).def -entry:MainEntryPoint -out:nsecdom.dll secdom.OBJ w_secdom.OBJ \
         $(ENTRY_FLAG1) $(LIBS_1) 

$(outfilename).obj : $(outfilename).c
        !cl $(COPTIONS) -Fosecdom.obj -DSEG_secdom secdom.c

w_secdom.OBJ: w_secdom.c
	!cl $(COPTIONS) -Fow_secdom.obj -DSEG_secdom w_secdom.c

