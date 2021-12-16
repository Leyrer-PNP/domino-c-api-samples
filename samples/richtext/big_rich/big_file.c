/************************************************************************
    
    PROGRAM:    BIG_FILE
                                                                    
    FILE:       BIG_FILE.C
                                                                        
    FUNCTION:   Create an ASCII text file of any size.                       
                                                                        
    SYNTAX:     BIG_FILE <filename> <count>                            
                                                                        
    DESCRIPTION:                                                        
        BIG_FILE is a utility for creating large ASCII files to be used  
        to test BIG_RICH, which takes an ASCII file as input.  BIG_FILE
        creates an ASCII file of the requested size broken into lines.
        Each line contains 72 characters terminated a by carriage-return,
        line-feed sequence.
                                                                        
*************************************************************************/

#include <stdio.h>
#if defined (LINUX)
#include <unistd.h>
#else
#include <io.h>
#endif

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <malloc.h>
#include <string.h>
#include <memory.h>                     /* _fmemset() */
#include <stdlib.h>                     /* atol() */

#define LINE_LEN    72


/************************************************************************
 *                                                                      *
 *  FUNCTION:   main()                                                  *
 *                                                                      *
 ************************************************************************/

int main(int argc, char *argv[])
{
    char          * file_name;
    unsigned long   count, l_count, c_count;
    int             hFile;
    unsigned long   i;
    char            line[LINE_LEN+1];

    if (argc != 3)
    {
        printf ("Usage: BIG_FILE  <filename>  <count> \n");
        exit(0);
    }
    file_name = argv[1];
    count = atol(argv[2]);

    if (count == 0)
    {
        printf ("Error: unable to convert '%s' to number.\n", argv[2]);
        exit(1);
    }

    /* Create file */
#if defined (LINUX)
	hFile = open (file_name, O_WRONLY| O_CREAT, S_IWRITE);
#else
	hFile = open(file_name, O_WRONLY | O_TEXT | O_CREAT, S_IWRITE);
#endif
       
    if (hFile == 0)
    {
        printf ("Error: unable to open file '%s' for write.\n", file_name);
        exit (1);
    }

    for (i=0; i<LINE_LEN; i++)
    {
        line[i] = (char)( (i % 10) + '0');
    }
    line[LINE_LEN] = '\n';

    l_count = count / (LINE_LEN+2) ;
    c_count = count % (LINE_LEN+2) ;
    printf("Writing %ld lines plus %ld characters.\n", l_count, c_count);

    for (i=0; i<l_count; i++)
    {
       if (write (hFile, line, LINE_LEN+1) == -1)
       {
           printf ("Error: write.\n");
           close(hFile);
           exit(1);
       }
    }

    if (write (hFile, line, (unsigned)c_count) == -1)
    {
        printf ("Error: write.\n");
        close(hFile);
        exit(1);
    }

    close(hFile);

    printf ("Created ASCII file '%s' containing %ld characters.\n",
                file_name, count);

    exit(0);
}
