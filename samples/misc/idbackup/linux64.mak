#
#  makefile for Notes API sample program idbackup
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include ../../../makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET = idbackup

# set SOURCES to the list of C source files in this program
SOURCES = $(TARGET).c

# set HEADERS to the list of C include files in this program
HEADERS =

# set OBJECTS to the list of object files that must be linked
OBJECTS = $(TARGET).o

BOOTOBJS = $(LOTUS)/notesapi/lib/notes0.o $(LOTUS)/notesapi/lib/notesai0.o

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS = -o $(TARGET)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS) $(OBJECTS) $(BOOTOBJS) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

