#
#  makefile for Notes API sample program PUTGET
#                Linux 
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include ../../../makeEnvLinux.mak

# set TARGET to the name of the executable to create

TARGET1 = executeputnote
TARGET2 = getnote
TARGET = $(TARGET1) $(TARGET2) 

# set SOURCES to the list of C source files in this program
SOURCES = get_ui.c  put_ui.c executeputnote.c getnote.c fields.c file_io.c misc.c

# set HEADERS to the list of C include files in this program
HEADERS =  constant.h putget.h  misc.h fileio.h fields.h

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = executeputnote.o
OBJECTS2 = getnote.o
OBJECTS3 = get_ui.o
OBJECTS4 = put_ui.o
OBJECTS5 = fields.o
OBJECTS6 = file_io.o
OBJECTS7 = misc.o 
OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7)


# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(OBJECTS1) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECTS2) $(OBJECTS3) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

