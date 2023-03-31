#
# This is an env and options that are
# common to the all Win makefiles
#

ENTRY_FLAG =  -entry:mainCRTStartup libvcruntime.lib ucrt.lib libcmt.lib

ENTRY_FLAG1 =  libvcruntime.lib ucrt.lib libcmt.lib

#Compilation options
COPTIONS = -nologo -c /MD /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl  -DNT -DW32 -DW -D_WINDOWS -DW64 -DND64 -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY
COPTIONS1 = -nologo -c  /Zi /Ot /O2 /Ob2 /Oy- -Gd /Gy /GF /Gs4096 /GS- /favor:INTEL64 /EHsc /Zc:wchar_t- /Zc:forScope- -Zl  -DNT -DW32 -DW -DW64 -DND64 -D_WINDOWS -D_AMD64_ -DDTRACE -D_CRT_SECURE_NO_WARNINGS -DND64SERVER -DPRODUCTION_VERSION  -DDUMMY

#Link options
LOPTIONS = -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpx  -debug -debugtype:cv -subsystem:windows 
LOPTIONS_1 = -nologo -opt:ref -machine:AMD64 -nodefaultlib /LARGEADDRESSAWARE -map:w64_cmp.mpx  -debug -debugtype:cv 

# set LIBS to list all the libraries ld should link with.
LIBS = notes.lib kernel32.lib user32.lib gdi32.lib advapi32.lib winspool.lib ws2_32.lib userenv.lib msvcrt.lib OLDNAMES.lib
LIBS_1 = notes.lib kernel32.lib user32.lib gdi32.lib advapi32.lib winspool.lib ws2_32.lib userenv.lib  

# Set Notes LIBS
NOTES_LIBS=notes0.obj notesai0.obj
