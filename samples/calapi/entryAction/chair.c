/****************************************************************************

    PROGRAM:    chair

    FILE:       chair.c

    SYNTAX:     chair serverName mailfile user1 user2

    PURPOSE:    demonstrates create of an iCalendar entry.

    DESCRIPTION:

        This program processes the following logic:

        .  Call TestCalCreateEntry to create an iCalendar entry in the
           mailbox.After this, you will see the entry in the calendar.
           if RRULE included, the entry repates.         

***************************************************************************/

#include "global.h"
#include "oserr.h"
#include "osfile.h"
#include "idtable.h"
#include "miscerr.h"
#include "nsf.h"
#include "osenv.h"
#include "osmem.h"
#include "ostime.h"
#include "osmisc.h"
#include "intl.h"
#include "calapi.h"
#include "textlist.h"
#include "names.h"
#include "kfm.h"
#include "icalSample.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ICAL_UID 	//Leaving it blank, if we don't provide the value, Notes will create one for you
#define OUTFILE		"EntryAction_UID.tmp"

#if defined(OS400)
#pragma convert(850)
#endif

// Global variables
FILE        *outFile=NULL;

void PrintAPIError (STATUS api_error)

{
    STATUS  string_id = ERR(api_error);
    char    error_text[200];
    WORD    text_len;
#if defined(OS390) && (__STRING_CODE_SET__!=ISO8859-1 /* ebcdic compile */)
    char    NATIVE_error_text[200];
#endif /* OS390, ebcdic compile */

    /* Get the message for this HCL C API for Domino and Notes error code
       from the resource string table. */

    text_len = OSLoadString (NULLHANDLE,
                             string_id,
                             error_text,
                             sizeof(error_text));

    /* Print it. */

#if defined(OS390) && (__STRING_CODE_SET__!=ISO8859-1 /* ebcdic compile */)
    OSTranslate(OS_TRANSLATE_LMBCS_TO_NATIVE, error_text, MAXWORD, NATIVE_error_text, sizeof(NATIVE_error_text));
    fprintf (stderr, "\n%s\n", NATIVE_error_text);
#else
    fprintf (stderr, "\n%s\n", error_text);
#endif /* OS390, ebcdic compile */

}

STATUS GetDBHdl(char* szPath, DHANDLE* hDB)
{
	STATUS error = NOERROR;
	char fullPath[MAXPATH] = "";
	
	OSPathNetConstruct(NULL, NULL, szPath, fullPath);
	error = NSFDbOpen(fullPath, hDB);

	if (error != NOERROR)
		PrintAPIError(error);

	return error;
}

STATUS TestCalCreateEntry(DHANDLE hDB, char* pszICalender, char* pOrganizer, char* pAttendee)
{
	STATUS	error = NOERROR;

	DWORD dwFlags = 0L;
	MEMHANDLE hRetUID = NULL;
	char* pszRetUID = NULL;
	
	error = CalCreateEntry(hDB, pszICalender, dwFlags, &hRetUID, NULL);
	if(error != NOERROR)
	{
		PrintAPIError(error);
		return error;
	}

	pszRetUID = (char*)OSMemoryLock(hRetUID);
   /* Print the note content to an open file */
    outFile = fopen( OUTFILE, "w" );
	fprintf (outFile, "%s\n", pszRetUID);
	fclose(outFile);
	
	printf("UID Returnd %s\n", pszRetUID);
	printf("Mail was sent to %s succesfully from %s\n", pAttendee, pOrganizer);

	OSMemoryUnlock(hRetUID);
	OSMemoryFree(hRetUID);
	return error;
}
	

int main(int argc, char *argv[])
{
	/*
	example from rfc5545

	"DTSTART:%s"  start at the next day, will be created by GetStartTime
	"RRULE:FREQ=DAILY;COUNT=10"  repeating 10 days. 
	"ORGANIZER:mailto:TestDomino/HCLPNP"  "test2 admin/HCLPNP" will be the chair of the meeting,change as you need
	"ATTENDEE;RSVP=TRUE:mailto:TestDomino/HCLPNP"   "test3 admin/HCLPNP" will be the attendee. If don't have RSVP=TRUE setted, will be a broadcast, can't be acceptted.
	*/
	char pszICalendartTemp[] = "BEGIN:VCALENDAR" CRLF
				"VERSION:2.0" CRLF
				"PRODID:-//hacksw/handcal//NONSGML v1.0//EN" CRLF
				"BEGIN:VEVENT" CRLF
				"UID:" ICAL_UID CRLF
				"DTSTAMP:20130820T172345Z" CRLF
				"DTSTART:%s" CRLF
				"DTEND:%s" CRLF
				"CATEGORIES:MEETING" CRLF
				"CLASS:PRIVATE" CRLF
				"RRULE:FREQ=DAILY;COUNT=10" CRLF
				"SUMMARY:Hot Summer Party" CRLF
				"ORGANIZER:mailto:%s" CRLF
				"ATTENDEE;RSVP=TRUE:mailto:%s" CRLF
				"END:VEVENT" CRLF
				"END:VCALENDAR" CRLF;

	char pszICalendar[1024] = {0};
	
	STATUS error = NOERROR;

	char pname[MAXPATH] = "";         /* buffer to store the input path to database */
	DHANDLE hDB = 0;

	
	char pStartTime[MAXALPHATIMEDATE] = {0};
	char pEndTime[MAXALPHATIMEDATE] = {0};
	char pRecurrenceID[MAXALPHATIMEDATE] = {0};
	char pOrganizer[MAXPATH] = {0};
	char pAttendee[MAXPATH] = {0};
	char psRetUID[MAXALPHATIMEDATE] = {0};
	
	if(argc < 5)
	{
		printf("usage: chair server1/test mail\\chair.nsf 'chair test/test' 'attendee test/test' \r\n");
		return 0;
	}
	error = NotesInitExtended (argc, argv);

	if (strcmp(argv[1], ""))
	{
		if (error = OSPathNetConstruct( NULL, argv[1], argv[2], pname))
		{
			PrintAPIError (error);
			NotesTerm();
			return (1);
		}
	}
	else
	{
		memcpy(pname, argv[2], sizeof(pname));
	}


	/* Get organizer mail id and attendee mail id */
	memcpy(pOrganizer, argv[3], sizeof(pOrganizer));
	memcpy(pAttendee, argv[4], sizeof(pAttendee));
	
	/*
	get start time of the first instance of the meeting
	*/
	GetStartTime(pStartTime, pEndTime, pRecurrenceID);
	sprintf(pszICalendar, pszICalendartTemp, pStartTime, pEndTime, pOrganizer, pAttendee);
	
	/*
	we should run as chair's id from now on. say "n if/test" here.
	*/
	error = GetDBHdl(pname, &hDB);
	if(error != NOERROR)
	{
		printf("GetDBHdl error!");
		goto ERROR_EXIT;
	}
	
	/*
	if create success, you will see the entry in Calendar.
	Note: when creating a meeting, Notes will send notice to attendees too.
		so make sure attendee name and server are all right.
	*/
	error = TestCalCreateEntry(hDB, pszICalendar, pOrganizer, pAttendee);
	if(error != NOERROR)
	{
		printf("TestCalCreateEntry error!");
		goto ERROR_EXIT;
	}

ERROR_EXIT:
	if(hDB)
	{
		NSFDbClose(hDB);
	}

	NotesTerm();

	return error;
}