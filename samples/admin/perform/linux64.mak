#
#  makefile for Notes API sample program perform
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include ../../../makeEnvLinux.mak

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

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)
LINKOPTS3 = -o $(TARGET3)
LINKOPTS4 = -o $(TARGET4)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS3) $(OBJECTS3) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS4) $(OBJECTS4) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

