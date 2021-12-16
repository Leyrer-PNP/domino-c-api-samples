/****************************************************************************
    PROGRAMS:   cdfread; cdfwrite

    FILE:       cdfile.h

****************************************************************************/

#if defined(UNIX)
   #define SIZE_T int
   #define FOPEN_MODE   "w"
#else
   #define SIZE_T size_t
   #define FOPEN_MODE   "w+b"
#endif

char CD_FILENAME[] =  "cd_recs.inp";

