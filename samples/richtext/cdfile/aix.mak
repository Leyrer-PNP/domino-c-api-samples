#
#  makefile for Notes API sample programs cdfwrite and cdfread.
#                 IBM AIX version
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
CC = xlC

# Set CCOPTS - the compiler options.
CCOPTS = -c
# You may use -g flag for debugging:
#CCOPTS = -c -g

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS1 and LINKOPTS2 to the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1) -bD:0x20000000
LINKOPTS2 = -o $(TARGET2) -bD:0x20000000

# Notes API header files require UNIX to be defined.
DEFINES =  -qcpluscmt -DUNIX -DAIX

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
# ***  FOR NOTES R4.1+ AIX THE NOTES LIBRARY IS libnotes_r.a ***
LIBS = -lnotes_r -lm

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
