#
#  makefile for Notes API sample program entryAction
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include $(NOTES_CAPI)/makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET1 = chair
TARGET2 = OneAttendee
TARGET = $(TARGET1) $(TARGET2)
# set SOURCES to the list of C source files in this program
SOURCE1 = $(TARGET1).c
SOURCE2 = $(TARGET2).c
SOURCES = $(SOURCE1) $(SOURCE2)   
# set HEADERS to the list of C include files in this program
HEADERS =

# set OBJECTS to the list of object files that must be linked
OBJECT1 = $(TARGET1).o 
OBJECT2 = $(TARGET2).o
OBJECTS = $(OBJECT1) $(OBJECT2)


# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(OBJECT1) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECT2) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

