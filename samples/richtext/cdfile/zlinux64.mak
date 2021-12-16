#
#  makefile for Notes API sample program cdfile
#               Linux for zSeries
#
#

# set TARGET to the name of the executable to create
TARGET1 = cdfwrite
TARGET2 = cdfread

# build two executables, cdfwrite and cdfread
all: cdfwrite cdfread

# set SOURCES to the list of C source files in this program
SOURCES1 = $(TARGET1).c
SOURCES2 = $(TARGET2).c

# set HEADERS to the list of C include files in this program
HEADERS = cdfile.h

# Set OBJECTS 1 and OBJECT2 to the list of object files created by this
# sample.
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o

# CC defines the compiler.
CC = gcc

# Set CCOPTS - the compiler options.
CCOPTS = -c -m64
# Use -g flag for debugging:
#CCOPTS = -c -g

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1) -m64
LINKOPTS2 = -o $(TARGET2) -m64

# Notes API header files require UNIX to be defined.
DEFINES =  -DUNIX -DLINUX -DZLINUX -DND64 -DW32 -DLINUX64 -DW -DZLINUX64 -DDPOOL_BLOCK_OPT  -DDTRACE -DPTHREAD_KERNEL -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -D_POSIX_THREAD_SAFE_FUNCTIONS  -DHANDLE_IS_32BITS -DHAS_IOCP -DHAS_BOOL -DHAS_DLOPEN -DUSE_PTHREAD_INTERFACES -DLARGE64_FILES -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -DTLOG_HARDWARE_COMPRESS -DNDUNIX64 -DLONGIS64BIT -DZSERIES64BIT  -DPRODUCTION_VERSION -DOVERRIDEDEBUG -fPIC

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
LIBS = -lnotes -lm -lnsl -lc -lpthread -lresolv -ldl

# the executable depends on the objects.
$(TARGET1): $(OBJECTS1)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

$(TARGET2): $(OBJECTS2)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
$(OBJECTS1): $(SOURCES1) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES1)

$(OBJECTS2): $(SOURCES2) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES2)

