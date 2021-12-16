/********************************************************************************************
 *
 *	PROGRAM:		DUSSPI.DLL
 *	
 *	MODULE:			DUSSPI.H
 *
 *	DESCRIPTION:	Constant definitions for DUS SPI DLL DUSSPI.DLL.
 *
 ********************************************************************************************
 */
typedef struct
{
	HMODULE                 hDUSModule;			/* Instance handle to this DLL */
	WORD                    ExtendedError;		/* User defined error, defined in duspierr.h */
	WORD                    ExtendedErrorLevel;	/* DUS_ERROR_LEVEL_xxx in DUSExtendedErrorText */
	DUSPROGRESSBARPROC      ProgressBarProc;	/* Progress Bar function pointer passed in with DUSStart */
	DUSLOGEVENTPROC         LogEventProc;		/* Log Event function pointer passes in with DUSStart */
}DUS_CONTEXT, *PDUS_CONTEXT;

#define CHAR_CR 13	
#define CHAR_LF 10
#define CHAR_SPC 32
#define CHAR_COMMA 44

