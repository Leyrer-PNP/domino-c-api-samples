/*************************************************************************
 *	Module:			CLFUNC.H
 *  Description:	External Function prototypes for module CLFUNC.C
 *					and referenced by module CLUMON.C
 *
 *************************************************************************/
extern STATUS GetServerLoad (char FAR*, DWORD FAR *);                    
extern STATUS GetServerCluster (char FAR*, char FAR*);	            
extern STATUS GetServerClusterMates (char FAR*, DWORD, HANDLE FAR *);  
extern STATUS GetDBMarks (char FAR*, char FAR*, DWORD*, BOOL*);  
extern STATUS SetDBMarks (char FAR*, char FAR*, WORD);  
extern STATUS RemoteCommand (char FAR*, char FAR*, char FAR*, WORD*);  
extern STATUS CreateAndCopyDB (char FAR*, char FAR*, char FAR*, char FAR*, DWORD);  
