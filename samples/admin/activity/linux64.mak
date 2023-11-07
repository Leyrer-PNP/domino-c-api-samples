#
#  makefile for Notes API sample program activity
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include $(NOTES_CAPI)/makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET = activity

# set SOURCES to the list of C source files in this program
SOURCES = $(TARGET).c

# set HEADERS to the list of C include files in this program
HEADERS =

# set OBJECTS to the list of object files that must be linked
OBJECTS = $(TARGET).o

# Link this program with the bootstrap code notes0.o because
# this program is structured as a NotesMain.
BOOTOBJS = $(NOTES_CAPI)/lib/notes0.o

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS = -o $(TARGET)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS) $(OBJECTS) $(BOOTOBJS) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

