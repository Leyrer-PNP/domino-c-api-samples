ReadMe for build.py contains details about script logic, compilation options and dependency.


This program is used to build the sample program as per the architecture of the python and OS on which it is running.

Following are the program logic,
-------------------------------
1. Identify the OS and system architecture
2. Identify the make file to execute in the platform
3. If any existing make.err or make.out file present under sample
   directory, then take a backup of that with current date and time(dd_mm_yy_H_M)
4. For all the directories and sub-directories, call compilation procedure
5. On each directory trigger corresponding makefile and redirect the output
   to make.err (all errors) and make.out (all logs)
6. Consolidate the compilation outputs under sample directory
7. Generate the report at the end of make.out file
8. Transfer the make.out & make.err to the jenkins machine (machine details passed as arguments)

Once this script completes then you could see the make.out and make.err file generated under samples folder.
This make.out and make.err will have all the commands that are triggered for compilation and its output.

On multiple runs on this script then you can see make.out & make.err, file will be backed-up with current date and time dd_mm_yy_H_M.

Command Line Options:
--------------------
   All the below mentioned options are optional parameters on a need basis include accordingly,
        -ip - Remote jenkins IP address. This is needed only to transfer files from one machine to another.
              Ex. -ip 127.0.0.1
        -u  - Remote jenkins user name.  This is needed only to transfer files from one machine to another.
              Ex. -u admin
        -c  - Perform binary and object file cleanup
              Ex. -c
        -a  - Architecture for compilation 32 or 64
              Ex. -a 32    or    -a 64

   Example usage is,
        32Bit machine:
        >python build.py -ip 127.0.0.1 -u admin -c -a 32

        64Bit machine:
        >python build.py -ip 127.0.0.1 -u admin -c -a 64

Note: Running build.py with "-c" options is recommended as it cleanup obj,pdb,exe in all the sample folders.

Dependency module:
------------------
This python script needs pysftp module to be installed before running the script.

   Steps to install pysftp moudle:

    1. Install python 2.7.18 or above
    2. This script needs pysftp module to be installed
       >pip install pysftp

Tested Platform:
---------------
    Windows 10
    RHEL
    CentOS



Sample make files are creation,
------------------------------
    make_17_09_2020_08_22.err  
    make_21_09_2020_07_01.err  
    make_21_09_2020_07_26.err  
    make_21_09_2020_13_42.err  
    make.err


Sample make.out file:
--------------------

./admin/acls/linux64.mak:
g++ -c -m64 -DGCC3 -DGCC4 -fno-strict-aliasing -DGCC_LBLB_NOT_SUPPORTED -Wformat -Wall -Wcast-align -Wconversion  -DUNIX -DLINUX -DLINUX86 -DND64 -DW32 -DLINUX64 -DW -DLINUX86_64 -DDTRACE -DPTHREAD_KERNEL -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -D_POSIX_THREAD_SAFE_FUNCTIONS  -DHANDLE_IS_32BITS -DHAS_IOCP -DHAS_BOOL -DHAS_DLOPEN -DUSE_PTHREAD_INTERFACES -DLARGE64_FILES -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -DNDUNIX64 -DLONGIS64BIT   -DPRODUCTION_VERSION -DOVERRIDEDEBUG  -fPIC  -I/opt/ibm/domino/notesapi/include acls.c
g++ -o acls acls.o -L/opt/ibm/domino/notes/latest/linux -Wl,-rpath-link /opt/ibm/domino/notes/latest/linux -lnotes -lm -lnsl -lc -lpthread -lresolv -ldl

`````````````Compilation Success```````````````````

./admin/activity/linux64.mak:
g++ -c -m64 -DGCC3 -DGCC4 -fno-strict-aliasing -DGCC_LBLB_NOT_SUPPORTED -Wformat -Wall -Wcast-align -Wconversion  -DUNIX -DLINUX -DLINUX86 -DND64 -DW32 -DLINUX64 -DW -DLINUX86_64 -DDTRACE -DPTHREAD_KERNEL -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -D_POSIX_THREAD_SAFE_FUNCTIONS  -DHANDLE_IS_32BITS -DHAS_IOCP -DHAS_BOOL -DHAS_DLOPEN -DUSE_PTHREAD_INTERFACES -DLARGE64_FILES -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -DNDUNIX64 -DLONGIS64BIT   -DPRODUCTION_VERSION -DOVERRIDEDEBUG  -fPIC  -I/opt/ibm/domino/notesapi/include activity.c
g++ -o activity activity.o /opt/ibm/domino/notesapi/lib/linux64/notes0.o -L/opt/ibm/domino/notes/latest/linux -Wl,-rpath-link /opt/ibm/domino/notes/latest/linux -lnotes -lm -lnsl -lpthread -lc -lresolv -ldl

`````````````Compilation failed````````````````````


Sample make.err file:
---------------------

setprivs.c:541:9: warning: ‘char* gets(char*)’ is deprecated (declared at /usr/include/stdio.h:638) [-Wdeprecated-declarations]
         gets(inputString);^M
         ^
setprivs.c:541:25: warning: ‘char* gets(char*)’ is deprecated (declared at /usr/include/stdio.h:638) [-Wdeprecated-declarations]
         gets(inputString);^M
                         ^
setprivs.c:559:7: warning: ‘char* gets(char*)’ is deprecated (declared at /usr/include/stdio.h:638) [-Wdeprecated-declarations]
       gets(inputString);^M
       ^
setprivs.c:559:23: warning: ‘char* gets(char*)’ is deprecated (declared at /usr/include/stdio.h:638) [-Wdeprecated-declarations]
       gets(inputString);^M
                       ^
setprivs.c:573:26: warning: conversion to ‘WORD {aka short unsigned int}’ from ‘int’ may alter its value [-Wconversion]
       *numEntries = argc - 4;^M
                          ^
setprivs.o: In function `ProcessArgs(int, char**, char*, char*, short*, unsigned short*, char**, unsigned short*, short*)':
setprivs.c:(.text+0x825): warning: the `gets' function is dangerous and should not be used.

`````````````Compilation Success```````````````````



Note: This build.py supports single execution or not multiple/parallel execution of build.py
