#
#  makefile for Notes API sample programs cdfwrite and cdfread.
#                Linux RedHat
#
#

# set TARGET to the name of the executable to create
TARGET1 = big_file
TARGET2 = big_rich

# build two executables, cdfwrite and cdfread
all: big_file big_rich

# set SOURCES1 and 2 to the names of the source files for these programs
SOURCES1 = $(TARGET1).c
SOURCES2 = $(TARGET2).c

# set HEADERS to the list of C include files in this program
HEADERS = 

# Set OBJECTS 1 and OBJECT2 to the list of object files created by this
# sample.
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o

# CC defines the compiler.
CC = g++

# Set CCOPTS - the compiler options.
CCOPTS = -c -m64
# You may use -g flag for debugging:
#CCOPTS = -c -mcpu=i486 -g

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)

# Notes API header files require UNIX to be defined.
DEFINES = -DGCC3 -DGCC4 -fno-strict-aliasing -DGCC_LBLB_NOT_SUPPORTED -Wformat -Wall -Wcast-align -Wconversion  -DUNIX -DLINUX -DLINUX86 -DND64  -DLINUX64 -DLINUX86_64 -DDTRACE -DPTHREAD_KERNEL -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -D_POSIX_THREAD_SAFE_FUNCTIONS  -DHANDLE_IS_32BITS -DHAS_IOCP -DHAS_BOOL -DHAS_DLOPEN -DUSE_PTHREAD_INTERFACES -DLARGE64_FILES -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -DNDUNIX64 -DLONGIS64BIT   -DPRODUCTION_VERSION -DOVERRIDEDEBUG  -fPIC 

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
LIBS = -lnotes -lm -lnsl -lpthread -lc -lresolv -ldl

# the executables depend on the objects.
$(TARGET1): $(OBJECTS1)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

$(TARGET2): $(OBJECTS2)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)


# the object files depend on the source files and header files.
$(OBJECTS1): $(SOURCES1) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES1)

$(OBJECTS2): $(SOURCES2) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES2)
