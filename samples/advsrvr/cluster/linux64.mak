#
#  makefile for Notes API sample program cluster
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include $(NOTES_CAPI)/makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET = clumon

# set SOURCES to the list of C source files in this program
SOURCES = clumoncm.c clfunc.c

# set HEADERS to the list of C include files in this program
HEADERS = clumon.h 

# set OBJECTS to the list of object files that must be linked
OBJECT1 = clumoncm.o 
OBJECT2 = clfunc.o
OBJECTS = $(OBJECT1) $(OBJECT2)

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPT = -o $(TARGET) 

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPT) $(OBJECTS)  -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)      
	

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

