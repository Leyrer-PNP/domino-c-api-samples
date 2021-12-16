TARGET1 = r_usrdef
TARGET2 = w_usrdef
TARGET = $(TARGET1) $(TARGET2)
SOURCES = $(TARGET1).c $(TARGET2).c proc_arg.c
HEADERS = user_def.h
OBJECTS1 = $(TARGET1).o proc_arg.o
OBJECTS2 = $(TARGET2).o proc_arg.o
CC = gcc

# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)
LIBS = -lnotes
INCDIR = $(LOTUS)/notesapi/include

CCOPTS = -x c -c -DGCC3 -DGCC4 -DGCC_LBLB_NOT_SUPPORTED -m32 -arch i386 -arch ppc -mmacosx-version-min=10.5 -dynamic  -O3 -fpascal-strings -Wmissing-declarations -Wmissing-prototypes -Wimplicit-function-declaration -Wno-deprecated-declarations -Wreturn-type

LINKOPTS1 = -o $(TARGET1) -m32 -felide-constructors
LINKOPTS2 = -o $(TARGET2) -m32 -felide-constructors

# Notes API header files require Macosx to be defined.
DEFINES = -DMAC -DMAC_OSX -DMAC_CARBON -D__CF_USE_FRAMEWORK_INCLUDES__ -DLARGE64_FILES -DHANDLE_IS_32BITS -DTARGET_API_MAC_CARBON -DTARGET_API_MAC_OS8=0   -DPRODUCTION_VERSION  -DOVERRIDEDEBUG

# the executable depends on the objects.
$(TARGET): $(OBJECTS1) $(OBJECTS2)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) $(LIBS) 
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) $(LIBS) 


# the object files depend on the corresponding source files.
.c.o:
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES)



