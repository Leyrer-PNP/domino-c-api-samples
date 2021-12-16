/*

	File:		EXTMNGR.r



	Contains:	Resources for the Extmngr



*/





#include "Types.r"

#include "SysTypes.r"

#include "CodeFragmentTypes.r"





resource 'vers' (1) {

	0x01,

	0x00,

	development,

	0x00,

	verUS,

	"1.00",

	"Version 1.00.  Copyright © 2020 HCL Technologies."

};





resource 'cfrg' (0) {

   {

      kPowerPC,				/* Target machine's Architecture. */

      kFullLib,				/* This is not an update. */

	  kNoVersionNum,		/* Current version. */

	  kNoVersionNum,		/* Definition version. */

	  kDefaultStackSize,	/* Stack size of application. */

	  kNoAppSubFolder,		/* Not used here.  Can be the resource-id of an 'alis'

	                           resource.  Used to provide additional location

							   to search for libraries. */

	  kIsLib,				/* This is library. */

	  kOnDiskFlat,          /* This code fragment is on disk, in the data fork. */

	  kZeroOffset,		    /* Offset of code into data fork. */

	  kWholeFork,           /* Code takes up all of data fork (can give a size). */

	  "EXTMNGR"    			/* Name of library. */

   }

};

