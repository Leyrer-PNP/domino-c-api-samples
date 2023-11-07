#
#  makefile for Notes API sample program billing
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include $(NOTES_CAPI)/makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET1 = billmgr.so  
TARGET2 = billsession
TARGET = $(TARGET1) $(TARGET2)
# set SOURCES to the list of C source files in this program
SOURCES = billmngr.c billses.c

# set HEADERS to the list of C include files in this program
HEADERS = 

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = billmngr.o
OBJECTS2 = billses.o

OBJECTS = $(OBJECTS1) $(OBJECTS2)

BOOTOBJS = $(NOTES_CAPI)/lib/notes0.o $(NOTES_CAPI)/lib/notesai0.o

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	ld $(LINKOPTS1) $(OBJECTS1)  -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECTS2) $(BOOTOBJS) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)	
	

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

