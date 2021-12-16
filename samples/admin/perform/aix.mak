#
#  makefile for Notes API sample program perform
#                 AIX version
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

# set HEADERS to the list of C include files in this program
HEADERS =

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o
OBJECTS3 = $(TARGET3).o
OBJECTS4 = $(TARGET4).o
OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4)

# CC defines the compiler.
CC = xlC

# Set CCOPTS - the compiler options.
CCOPTS = -c
# You may use -g flag for debugging:
#CCOPTS = -c -g

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1) -bD:0x20000000
LINKOPTS2 = -o $(TARGET2) -bD:0x20000000
LINKOPTS3 = -o $(TARGET3) -bD:0x20000000
LINKOPTS4 = -o $(TARGET4) -bD:0x20000000

# Notes API header files require UNIX to be defined.
DEFINES =  -qcpluscmt -DUNIX -DAIX

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
# ***  FOR NOTES R4.1+ AIX THE NOTES LIBRARY IS libnotes_r.a ***
LIBS = -lnotes_r -lm

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS3) $(OBJECTS3) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS4) $(OBJECTS4) -L$(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES)
