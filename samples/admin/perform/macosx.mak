# set TARGET to the name of the executable to create
TARGET1 = addnames
TARGET2 = index
TARGET3 = mixed_1
TARGET4 = mixed_2
TARGET = $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)




# set SOURCES to the list of C source files in this program
SOURCES = $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c

# set OBJECTS to the list of object files that must be linked
OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o
OBJECTS3 = $(TARGET3).o
OBJECTS4 = $(TARGET4).o
OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4)

CC = gcc

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)
LIBS = -lnotes
INCDIR = $(LOTUS)/notesapi/include

CCOPTS = -x c -c -DGCC3 -DGCC4 -DGCC_LBLB_NOT_SUPPORTED -m32 -arch i386 -arch ppc -mmacosx-version-min=10.5 -dynamic  -O3 -fpascal-strings -Wmissing-declarations -Wmissing-prototypes -Wimplicit-function-declaration -Wno-deprecated-declarations -Wreturn-type

LINKOPTS = -m32 -felide-constructors
LINKOPTS1 = -o $(TARGET1)
LINKOPTS2 = -o $(TARGET2)
LINKOPTS3 = -o $(TARGET3)
LINKOPTS4 = -o $(TARGET4)

# Notes API header files require Macosx to be defined.
DEFINES = -DMAC -DMAC_OSX -DMAC_CARBON -D__CF_USE_FRAMEWORK_INCLUDES__ -DLARGE64_FILES -DHANDLE_IS_32BITS -DTARGET_API_MAC_CARBON -DTARGET_API_MAC_OS8=0   -DPRODUCTION_VERSION  -DOVERRIDEDEBUG

# the executable depends on the objects.
$(TARGET): $(OBJECTS)
	$(CC) $(LINKOPTS1) $(LINKOPTS) $(OBJECTS1) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS2) $(LINKOPTS) $(OBJECTS2) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS3) $(LINKOPTS) $(OBJECTS3) -L$(NOTESDIR) $(LIBS)
	$(CC) $(LINKOPTS4) $(LINKOPTS) $(OBJECTS4) -L$(NOTESDIR) $(LIBS)

# the object files depend on the corresponding source files.
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES)



