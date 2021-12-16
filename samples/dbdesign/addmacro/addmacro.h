/****************************************************************************
    (c) Copyright IBM Corporation   1996,   2017
    (c) Copyright HCL Technologies   1996,   2021

    PROGRAM: addmacro

    FILE:    addmacro.h

****************************************************************************/

/* Constants */
#define  MAX_FLAGS   32

/* Function Prototypes */
STATUS  LNPUBLIC  AddFilterMacro( DBHANDLE );
STATUS  LNPUBLIC  AddBackgroundMacro( DBHANDLE );
STATUS  LNPUBLIC  AddOnceMacro( DBHANDLE );
STATUS  LNPUBLIC  CreateMacroNote ( DBHANDLE, NOTEHANDLE* );
STATUS  LNPUBLIC  SetMacroTitle( NOTEHANDLE, char * );
STATUS  LNPUBLIC  SetMacroComment( NOTEHANDLE, char * );
STATUS  LNPUBLIC  SetMacroFormula( NOTEHANDLE, char * );
STATUS  LNPUBLIC  SetMacroType( NOTEHANDLE, WORD );
STATUS  LNPUBLIC  SetMacroOperation( NOTEHANDLE, WORD );
STATUS  LNPUBLIC  SetMacroScan( NOTEHANDLE, WORD );
STATUS  LNPUBLIC  SetMacroFlags( NOTEHANDLE, char * );
STATUS  LNPUBLIC  SetMacroMachineName( NOTEHANDLE );
STATUS  LNPUBLIC  SetMacroPeriod( NOTEHANDLE, WORD );
STATUS  LNPUBLIC  SetMacroLeftToDo( DBHANDLE, NOTEHANDLE );


