#
#  makefile for Notes API sample program billing
#                 AIX version
#
#

# set TARGET to the name of the executable to create
TARGET1 = libbillmgr.a
TARGET2 = billses
TARGET = $(TARGET1) $(TARGET2)

# set SOURCES to the list of C source files in this program
SOURCES = billmngr.c $(TARGET2).c

# set HEADERS to the list of C include files in this program
HEADERS = bill_ext.h billses.h

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = billmngr.o
OBJECTS2 = $(TARGET2).o

# Link this program with the bootstrap code notes0.o because
# this program is structured as a NotesMain.
BOOTOBJS = $(LOTUS)/notesapi/lib/aix/notes0.o $(LOTUS)/notesapi/lib/aix/notesai0.o

# CC defines the compiler.
CC = xlC

# Set CCOPTS - the compiler options.
CCOPTS = -c -g
# You may use -g flag for debugging:
#CCOPTS = -c -g

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1) -bE:exports.aix -bM:SRE -bnoentry
LINKOPTS2 = -o $(TARGET2) -bD:0x20000000

# Notes API header files require UNIX to be defined.
DEFINES =  -qcpluscmt -DUNIX -DAIX

# set INCDIR to specify where to search for include files
INCDIR = $(LOTUS)/notesapi/include

# set LIBS to list all the libraries ld should link with.
# ***  FOR NOTES R4.1+ AIX THE NOTES LIBRARY IS libnotes_r.a ***
LIBS1 = -lnotes_r -lm -lc_r
LIBS2 = -lnotes_r -lm

# the executable depends on the objects.
$(TARGET): $(OBJECTS1) $(OBJECTS2)
	ld $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) $(LIBS1)
	$(CC) $(LINKOPTS2) $(OBJECTS2) $(BOOTOBJS) -L$(NOTESDIR) $(LIBS2)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES)
