/* DESCRIPTION

This include file contains the function prototype for miscellaneous functions in
the PUTNOTE/GETNOTE package.

*/

extern void datatype_bin2ascii (USHORT, char *);

extern void datatype_ascii2bin (char *, USHORT *);

extern int set_control_info (char *);

extern char get_record_separator (void);

extern char get_field_separator (void);

extern char get_list_separator (void);
