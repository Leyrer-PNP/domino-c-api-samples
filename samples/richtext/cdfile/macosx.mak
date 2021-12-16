TARGET1 = cdfwrite
TARGET2 = cdfread

all: cdfwrite cdfread
SOURCES1 = $(TARGET1).c
SOURCES2 = $(TARGET2).c
CC = gcc

HEADERS = cdfile.h

OBJECTS1 = $(TARGET1).o
OBJECTS2 = $(TARGET2).o


# set NOTESDIR to specify where to search for the Notes library file
NOTESDIR = $(Notes_ExecDirectory)
LIBS = -lnotes
INCDIR = $(LOTUS)/notesapi/include

CCOPTS = -x c -c -DGCC3 -DGCC4 -DGCC_LBLB_NOT_SUPPORTED -m32 -arch i386 -arch ppc -mmacosx-version-min=10.5 -dynamic  -O3 -fpascal-strings -Wmissing-declarations -Wmissing-prototypes -Wimplicit-function-declaration -Wno-deprecated-declarations -Wreturn-type

LINKOPTS1 = -o $(TARGET1) -m32 -felide-constructors
LINKOPTS2 = -o $(TARGET2) -m32 -felide-constructors

# Notes API header files require Macosx to be defined.
DEFINES = -DMAC -DMAC_OSX -DMAC_CARBON -D__CF_USE_FRAMEWORK_INCLUDES__ -DLARGE64_FILES -DHANDLE_IS_32BITS -DTARGET_API_MAC_CARBON -DTARGET_API_MAC_OS8=0   -DPRODUCTION_VERSION  -DOVERRIDEDEBUG


# the object files depend on the objects.
$(TARGET1): $(OBJECTS1)
	$(CC) $(LINKOPTS1) $(OBJECTS1) -L$(NOTESDIR) $(LIBS)

$(TARGET2): $(OBJECTS2)
	$(CC) $(LINKOPTS2) $(OBJECTS2) -L$(NOTESDIR) $(LIBS)


# the object files depend on the source files and header files.
$(OBJECTS1): $(SOURCES1) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES1)

$(OBJECTS2): $(SOURCES2) $(HEADERS)
	$(CC) $(CCOPTS) $(DEFINES) -I$(INCDIR) $(SOURCES2)

