#
#  makefile for Notes API sample program perform
#                 AIX 64-bit version
#
#

# set TARGET to the name of the executable to create
TARGET1 = addnames
TARGET2 = index
TARGET3 = mixed_1
TARGET4 = mixed_2
TARGET = $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)

# set SOURCES to the list of C source files in this program
SOURCES = $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c



# set OBJECTS to the list of object files that must be linked
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o
OBJECTS3 = $(TARGET3).o
OBJECTS4 = $(TARGET4).o
OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4)

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

# Set LINKOPTS - the linker options passed to CC when linking.


LINKOPTS = -q64 -qarch=ppc -I/usr/include -I/usr  -O -qalias=noansi -qsuppress=0711-224:0711-345
LINKOPTS1 = -o $(TARGET1) 
LINKOPTS2 = -o $(TARGET2)
LINKOPTS3 = -o $(TARGET3)
LINKOPTS4 = -o $(TARGET4)

# Notes API header files require UNIX to be defined.
DEFINES = -DUNIX -DAIX -DW -DW32 -DAIX64 -DND64 -D_ALL_SOURCE -DNDUNIX64 \
   -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -DIBM_AIX \
   -DAIX_ATOMICS -DHAS_DLOPEN -DHAS_BOOL -DLARGE64_FILES \
   -D_LARGE_FILE_API -DNO_OVERLOAD_DEL_OPERATOR -DHANDLE_IS_32BITS \
   -DND64 -DNDUNIX64 -DPRODUCTION_VERSION -DOVERRIDEDEBUG

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
LIBS = -lnotes_r -lm -L/usr/lib/threads \
   -L/usr/lib -lC_r -lc_r -lpthreads -ltli_r

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(LINKOPTS) $(OBJECTS1) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(LINKOPTS) $(OBJECTS2) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS3) $(LINKOPTS) $(OBJECTS3) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS4) $(LINKOPTS) $(OBJECTS4) -L$(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES)
