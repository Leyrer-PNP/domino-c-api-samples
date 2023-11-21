#
#  makefile for Notes API sample program secdom
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include ../../../makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET = libsecdom.so

# set SOURCES to the list of C source files in this program
SOURCES = secdom.c u_secdom.c

# set HEADERS to the list of C include files in this program
HEADERS =

# set OBJECTS to the list of object files that must be linked
OBJECTS = secdom.o u_secdom.o

# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS = -o $(TARGET)

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	ld $(LINKOPTS) $(OBJECTS) -L$(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

