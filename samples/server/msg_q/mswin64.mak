#
# makefile for Notes API sample program MSG_Q
# Windows 64-bit version using
# Microsoft Visual Studio 2010 SP1
#
# This makefile assumes that the INCLUDE and LIB environment variables
# are set up to point at the Notes and C "include" and "lib" directories.

# Windows 64-bit make definitions
!include <makeEnvWin.mak>

##
outfilename1 = msg_q
outfilename2 = msg_t

# targets
all: $(outfilename1).exe $(outfilename2).exe

# Update the executable file if necessary
$(outfilename1).exe: msg_q.obj
	link $(LOPTIONS)  msg_q.obj $(NOTES_LIBS) \
		$(LIBS) $(ENTRY_FLAG) -out:$(outfilename1).exe

# Update the object file if necessary.
msg_q.obj : msg_q.c
	!cl $(COPTIONS)  msg_q.c

# Update the executable file if necessary
$(outfilename2).exe: msg_t.obj
	link $(LOPTIONS) msg_t.obj $(NOTES_LIBS) \
	$(ENTRY_FLAG)	$(LIBS) -out:$(outfilename2).exe

# Update the object file if necessary.
msg_t.obj : msg_t.c
	!cl $(COPTIONS)   msg_t.c
