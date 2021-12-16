/* DESCRIPTION

This header file contains declarations for the platform-dependent
file I/O functions.

*/

extern int open_textfile (char *, int);
extern void close_textfile (void);

extern int open_logfile (char *);
extern void close_logfile (void);

extern void write_text_string (char *);
extern void write_text_char (char);

extern int read_text_char (char *);
extern void unread_text_char (char);

extern void log_message (char *, char *);
extern void log_api_message (char *, STATUS);
