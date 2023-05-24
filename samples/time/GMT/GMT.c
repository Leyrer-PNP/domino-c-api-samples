/****************************************************************************
 *
 * Copyright HCL Technologies 1996, 2023.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *

    PROGRAM:    Gmt

    FILE:       Gmt.c

    SYNTAX:     Gmt 

    PURPOSE:    Demonstrates how to use the time functions

    DESCRIPTION:
            With the given sample date and time extracted and compared with time
        derived from different date and time functions.
        
        parameters compared are  date,time, daylight saving time and timezone 

        APIs explored : TimeExtractTicks,TimeExtractDate,TimeGMToLocalZone.
                        TimeGMToLocal
        
****************************************************************************/
/*    C Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Notes Includes */
#include <global.h>
#include <osmisc.h>
#include <log.h>
#include <osfile.h>
#include <ostime.h>
#include <misc.h>


#define ZONE_MASK                0x0f000000L
#define DST_MASK                0x80000000L
#define DATE_MASK                0x00ffffffL
#define ZONE_HOUR_MIN_MASK        0x7f000000
#define CLEANUP_NOTHING            0x0000
/*Number of bits to be shifted to get Most significant byte from word */
#define BIT_SHIFT_COUNT            24
/* Below macros are  re-used from misc    ime.c of Notes source code */
#define    ZONE_MINUTE_GRANULARITY    15
#define    ZONE_HOURS(x)            ((x)&0x0f)
#define    ZONE_MINUTES(x)            ((((x)&0x30)>>4)*ZONE_MINUTE_GRANULARITY)
#define    ZONE_EAST_OF_GMT(x)        (((x)&0x40)!=0)


STATUS after_cleanup(STATUS exit_status);

/* Global Variables */
WORD    cleanup = CLEANUP_NOTHING;

int main(int argc, char *argv[])
{

    STATUS      error;                /* Notes return code variable */
    DWORD     dwDate; 
    DWORD     dwTime;
    TIMEDATE  tdNowTime;
    TIMEDATE  tdReturnTime;
    TIME tTime;
    int       iTimeZone, iDst, iGMT_hour, iGMT_min, iGMT_datetime;
     

    printf("Test of TimeExtractDate\n");

    /* Start Notes */
    if(error = NotesInitExtended (argc, argv)){
        printf("\n Unable to initialize Notes.\n");
        return (1);
    }

    /* Reading current Timezone and Dayy light saving time */
    printf("Calling OSCurrentTimeZone to get the time zone\n");
    OSCurrentTimeZone(&iTimeZone, &iDst);
  
    /* Reading Current time into tdNowTime */
    printf("Calling OSCurrentTIMEDATE to put something in tdNowTime\n");
    OSCurrentTIMEDATE(&tdNowTime);

    /* Extracting ticks from current time */
    printf("Calling TimeExtractTicks to get the Time part\n");
    dwTime = TimeExtractTicks(&tdNowTime);

    /* Extracting date from current time including Timezone and DST */
    printf("Calling TimeExtractDate to get the Date part\n");
    dwDate = TimeExtractDate(&tdNowTime);
  
    printf("TimeExtractDate returned with %#8.8x\n", dwDate);

    printf("Comparing OS DST to returned DST\n");

    /* Extract DST from Date and compare it to value returned by OSCurrentTimeZone */

    if(iDst != (int) ((dwDate & DST_MASK)>>31)){
        printf("Daylight Savings Times do not match - FAILED\n");
        return(after_cleanup(-1));
    }
    printf("Daylight Savings Times match - VERIFIED\n");  

    /* Mask off the time zone and Daylight Savings Time (Probably not needed)*/

    dwDate = dwDate & DATE_MASK;

    printf("Calling TimeConstruct with Date = %#x Time = %#x\n",
            (ULONG)dwDate, (ULONG)dwTime);
    TimeConstruct(dwDate, dwTime, &tdReturnTime);

    printf("Comparing original TIMEDATE to returned TIMEDATE\n");
    if(!TimeDateEqual(&tdNowTime, &tdReturnTime)){
        printf( "TIMEDATEs do not match - FAILED\n");
        return(after_cleanup(-1));
    }
    printf("TIMEDATEs match - VERIFIED\n");

    /* converting current timezone into GMT timeZone */
     
    memcpy(&(tTime.GM), &tdNowTime, sizeof(TIMEDATE));
    tTime.zone = -3005; /* GMT+5:30 */
    tTime.dst = 0;
    TimeGMToLocalZone(&tTime);
    printf("hour : %d minute : %d second : %d \n ", tTime.hour, tTime.minute, tTime.second);
    tTime.zone = 0; 
    tTime.dst = 0;
    TimeGMToLocalZone(&tTime);
    printf("hour : %d minute : %d second : %d \n ", tTime.hour, tTime.minute, tTime.second);
    TimeGMToLocal(&tTime);
    printf("hour : %d minute : %d second : %d \n", tTime.hour, tTime.minute, tTime.second);
    

    return(after_cleanup(0));
}


/*
* after_cleanup: Cleans up resources and return the current exit status
*
*     in: Global cleanup with various flags set
*
*    out: As described
*/
STATUS after_cleanup(STATUS exit_status)
{

  /* Done with Notes */
    NotesTerm();

    return(exit_status);
}
