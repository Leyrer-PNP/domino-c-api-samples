/* DESCRIPTION

This include file contains prototypes for low-level functions that
read or write specific types of Domino and Notes fields.

These functions are used in the GETNOTE and PUTNOTE programs.
*/


/* Function Prototypes */

extern WORD get_text_field (BLOCKID, WORD, char *, WORD);

extern int put_text_field (NOTEHANDLE, char *, char *);

extern int put_textlist_field (NOTEHANDLE, char *, char *);

extern int put_number_field (NOTEHANDLE, char *, char *);

extern int put_numberlist_field (NOTEHANDLE, char *, char *);

extern int put_time_field (NOTEHANDLE, char *, char *);

extern int put_timelist_field (NOTEHANDLE, char *, char *);

extern int put_richtext_field (NOTEHANDLE, char *, char *);

extern int put_userid_field (NOTEHANDLE, char *, char *);
