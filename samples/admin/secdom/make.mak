.names: . @@text.nam

;       The following NAME is used for the executable, and will be prefixed with
;       a $ in OS/2 and _ in DOS.  It must be a maximum of 7 characters.  The
;       DESC comment is put into the EXE itself.

#define NAME secdom

;       Use "ifndef DOSCHAR" for both DOS and OS/2 versions, or
;       Use "ifdef OS2" for just the OS/2 version

;   Compile source modules

#group pass2
#group main

@c.nam([OBJ]): %.c
	[$ccsdk] [$seg]secdom %.c

#endgroup main
#endgroup pass2

;
;       Listings
;

#ifdef $listing
@c.nam([LIS]): . %.c
	>[$ccsdk] [$listing] %.c
#endif


;       Resources Files

#group pass2

#ifdef $RC
secdom.[RES]: @rc.nam [NOTESSDK]\globerr.h secdom.h
	$ echo [$cppsdk] %%1 %%)%%)[TM].rc >[TM].dat
	build [TM].dat @rc.nam -o [TM].[CMD]
	[$delete] [TM].dat
	$ if exist [TM].rc [$delete] [TM].rc
	$ [TM]
	[$delete] [TM].[CMD]
	> [$RC] -r [TM].rc
	[$delete] [TM].rc
	[$cvtres] [TM].res [$cvtresout] $@
	[$delete] [TM].res
#endif $RC

#ifdef UNIX
[NOTESBIN]/[NAME].res: [NOTESSDK]/globerr.h secdom.h
    $ echo "#define [ENV]" >[TM].rc
    $ echo "#define W" >> [TM].rc
    $ echo "#define W16" >> [TM].rc
    $ cat mtbf.rc >>[TM].rc
;** Generate .RES
    $ [NOTESKIT]/rc.sh [TM].rc $@
    [$delete] [TM].rc
#endif UNIX

#endgroup pass2

;
;       executables for NT
;

;
;	shared library for NT
;

[NOTESBIN]\[P][NAME].dll: #main make.mak
	$ build @c.nam -x -l.[OBJ] >obj.[TM]
	$ cat mswin64.def >def.[TM]
	[$LIBR] -out:[NOTESLIB]\$/.lib @obj.[TM] -def:def.[TM]
	> [$LINKDLL] -out:$@ [NOTESLIB]\$/.exp @obj.[TM] [$LINKDLLEND] [$LINKLIBNOTES] [$LINKDLLSHAREDCRTEND]
;	> [$LINKDLL] -out:$@ @obj.[TM] [$LINKDLLEND] [$LINKLIBNOTES] 
	[$delete] obj.[TM]

[NOTESBIN]\[P][NAME].[SYM]: [SYM?] %.dll
	[$MAPSYM] $/.[MAP]
	[$copy] $/.[SYM] $@
	[$delete] $/.[SYM]

;   Request to compile all sources


#group pass1
#endgroup pass1

compile: . #pass1
compile.[OBJ]: . #pass1
	@$ echo Compiled all object modules

#group pass2
#endgroup pass2

prelink: . #pass2
prelink.[OBJ]: . #pass2
	@$ echo Built all import libraries



