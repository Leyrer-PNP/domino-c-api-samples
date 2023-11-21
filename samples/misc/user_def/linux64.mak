#
#  makefile for Notes API sample program user_def
#                Linux RedHat
#  NOTE: set NOTES_CAPI env before executing makefile
#

#make env file
include ../../../makeEnvLinux.mak

# set TARGET to the name of the executable to create
TARGET1 = r_usrdef
TARGET2 = c_usrdef
TARGET = $(TARGET1) $(TARGET2)

# set SOURCES to the list of C source files in this program
SOURCES = r_usrdef.c c_usrdef.c proc_arg.c

# set HEADERS to the list of C include files in this program
HEADERS = $(TARGET).h

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = r_usrdef.o proc_arg.o
OBJECTS2 = c_usrdef.o proc_arg.o


# Set LINKOPTS - the linker options passed to CC when linking.
# -o $(TARGET) causes compiler to create target rather than a.out
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)

# the executable depends on the objects.
$(TARGET): $(OBJECTS1)  $(OBJECTS2)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) -Wl,-rpath-link $(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) $(INCDIR) $(SOURCES)

