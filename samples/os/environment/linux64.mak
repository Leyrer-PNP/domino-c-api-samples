#
#  Makefile for Notes API sample program environment
#                Linux
#  NOTE: set NOTES_CAPI env before executing makefile
#


#make env file
include ../../../makeEnvLinux.mak
# Set TARGET to the name of the executable to create.
TARGET = environment

# Set SOURCES to the list of C source files in this program.
SOURCES = $(TARGET).c

# Set HEADERS to the list of C include files in this program.
HEADERS =

# Set OBJECTS to the list of object files that must be linked.
OBJECTS = $(TARGET).o


# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS = -o $(TARGET)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS) $(OBJECTS) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# The object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

