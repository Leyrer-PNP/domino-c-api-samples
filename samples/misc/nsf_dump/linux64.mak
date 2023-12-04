#
#  makefile for Notes API sample program nsf_dump
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include ../../../makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET = nsf_dump

# set SOURCES to the list of C source files in this program
SOURCES = $(TARGET).c dumpitem.c dumpcd.c cdfile.c agents.c navigate.c mime.c

# set HEADERS to the list of C include files in this program
HEADERS = nsf_dump.h

# set OBJECTS to the list of object files that must be linked
OBJECTS = $(TARGET).o dumpitem.o dumpcd.o cdfile.o agents.o navigate.o mime.o


# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS = -o $(TARGET)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS) $(OBJECTS) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

