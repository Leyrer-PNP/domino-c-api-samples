
/*
 * SEL_REP.H
 */

#ifndef __SEL_REP_H
#define __SEL_REP_H

#define TEXTLIST_BUFFER_LENGTH  1024
#define CLASS_STRING_LENGTH     10

/*
 *  Define the classes of notes that the sample program is interested
 *  in replicating.
 */
 
#define MY_NOTE_CLASSES (NOTE_CLASS_DOCUMENT | \
                         NOTE_CLASS_INFO | \
                         NOTE_CLASS_FORM | \
                         NOTE_CLASS_VIEW | \
                         NOTE_CLASS_ICON | \
                         NOTE_CLASS_DESIGN | \
                         NOTE_CLASS_ACL | \
                         NOTE_CLASS_HELP_INDEX | \
                         NOTE_CLASS_HELP | \
                         NOTE_CLASS_FILTER | \
                         NOTE_CLASS_FIELD | \
                         0)


#endif

