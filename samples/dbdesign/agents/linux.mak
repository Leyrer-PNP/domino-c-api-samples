#
#  makefile for Notes API sample program agents/ragents
#                Linux RedHat
#
#

# set TARGET to the name of the executable to create
TARGET1 = agents
TARGET2 = ragents
TARGET = $(TARGET1) $(TARGET2)

# set SOURCES to the list of C source files in this program
SOURCES = $(TARGET1).c $(TARGET2).c

# set HEADERS to the list of C include files in this program
HEADERS =

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o
OBJECTS = $(OBJECTS1) $(OBJECTS2)

# CC defines the compiler.
CC = g++

# Set CCOPTS - the compiler options.
CCOPTS = -c -mcpu=i486
# You may use -g flag for debugging:
#CCOPTS = -c -mcpu=i486 -g

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)

# Notes API header files require UNIX to be defined.
DEFINES =  -DUNIX -DLINUX -DLINUX86 -DW -DW32 -DDTRACE -fcheck-new -DPTHREAD_KERNEL -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -D_POSIX_THREAD_SAFE_FUNCTIONS  -DHANDLE_IS_32BITS -DHAS_IOCP -DHAS_BOOL -DHAS_DLOPEN -DUSE_PTHREAD_INTERFACES -DLARGE64_FILES -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE   -DPRODUCTION_VERSION -DOVERRIDEDEBUG  -fPIC

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
LIBS = -lnotes -lm -lnsl -lpthread -lc -lresolv -ldl

$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES)
