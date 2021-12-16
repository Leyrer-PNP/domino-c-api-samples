#
#  makefile for Notes API sample programs cdfwrite and cdfread.
#                 IBM AIX 64-bit version
#
# This makefile builds two executable files: (1) cdfwrite and
# (2) cdfread. The pseudotarget, all, is used to build two target
# executables.
##

# set TARGET to the name of the executable to create
TARGET1 = cdfwrite
TARGET2 = cdfread

# build two executables, cdfwrite and cdfread
all: cdfwrite cdfread

# set SOURCES1 and 2 to the names of the source files for these programs
SOURCES1 = $(TARGET1).c
SOURCES2 = $(TARGET2).c

# set HEADERS to the list of C include files in this program
HEADERS = cdfile.h

# Set OBJECTS 1 and OBJECT2 to the list of object files created by this
# sample.
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o

# CC defines the compiler.
CC = xlc_r

# Set CCOPTS - the compiler options.
CCOPTS = -c -g -q64 \
   -qsuppress=1506-356:1500-010:1506-280  -c -qfullpath -qtbtable=full \
   -qstaticinline -qchars=unsigned -qsrcmsg -qthreaded -qcpluscmt \
   -qhalt=s -O -qalias=noansi -qarch=ppc -qMAXMEM=8192 \
   -qlargepage -qnoinline


# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS1 and LINKOPTS2 to the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out

LINKOPTS = -q64\
   -qarch=ppc -I/usr/include -I/usr  -O -qalias=noansi \
   -qsuppress=0711-224:0711-345
LINKOPTS1 = -o $(TARGET1) $(LINKOPTS)
LINKOPTS2 = -o $(TARGET2) $(LINKOPTS)

# Notes API header files require UNIX to be defined.
DEFINES = -DUNIX -DAIX -DW -DW32 -DAIX64 -DND64 -D_ALL_SOURCE -DNDUNIX64 \
   -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -DIBM_AIX \
   -DAIX_ATOMICS -DHAS_DLOPEN -DHAS_BOOL -DLARGE64_FILES \
   -D_LARGE_FILE_API -DNO_OVERLOAD_DEL_OPERATOR -DHANDLE_IS_32BITS \
   -DND64 -DNDUNIX64 -DPRODUCTION_VERSION -DOVERRIDEDEBUG

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
# ***  FOR NOTES R4.1+ AIX THE NOTES LIBRARY IS libnotes_r.a ***

LIBS = -lnotes_r -lm -L/usr/lib/threads \
   -L/usr/lib -lC_r -lc_r -lpthreads -ltli_r

# dependencies for cdfwrite and cdfread

$(TARGET1): $(OBJECTS1)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) $(LIBS)

$(TARGET2): $(OBJECTS2)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) $(LIBS)


# the object files depend on the source files and header files.
$(OBJECTS1): $(SOURCES1) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES1)

$(OBJECTS2): $(SOURCES2) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES2)
