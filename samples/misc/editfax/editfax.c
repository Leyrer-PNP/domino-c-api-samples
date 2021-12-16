
/* This sample program demonstrates the usage of the two Editfax APIs:

      EditorPrintNoteToBitmap and EditorAppendBitmapToNote
       
   The program converts note(s) in the source database to FAX bitmap(s).  
   Then, it appends the FAX bitmap(s)to new note(s) in the target database.
   The names of the source and the target database are hard coded in the
   variables SOURCEDB, TARGETDB correspondingly.

*/

#define   GUI_PAINT


/* HCL C API for Notes/Domino include files */

#include <global.h>
#include <osmisc.h>
#include <oserr.h>
#include <misc.h>
#include <pool.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <editfax.h>
#include <editods.h>
#include <editdflt.h>
#include <nsfsearc.h>    /* SEARCH_MATCH, etc. */
#include <osmisc.h>

/* OS include files */
#ifndef UNIX
#include <wingdi.h> 
#endif
#include <stdio.h>
#include <stdlib.h>

#if !defined(ND64) 
    #define DHANDLE HANDLE 
#endif
 
#define DPI              200
#define WIDTH            8.5
#define HEIGHT           11
#define RGBVALUE_BLACK   0x00000000L
#define COMPANYNAME     "Editfax test"

#define FORMNAME        "Simple Form"
#define TARGETDB        "makeform.NSF"
#define SOURCEDB        "faxsrc.NSF"


/*   Context structure */

EPBCONTEXT EPBContext;
         
DBHANDLE hSourceDB;
DBHANDLE hTargetDB;
DHANDLE hTargetNote;


/*   Forward routines */

STATUS near PASCAL FaxDocument (NOTEID SourceNoteID);
STATUS LNCALLBACK  FaxEnumerationCallback (void *dbHandle,
                   SEARCH_MATCH *pSearchMatch, ITEM_TABLE *pViewBuffer);
BOOL LNCALLBACK    ProcessBitmap (EPBCONTEXT *EPBContext, WORD PageNumber);
HBITMAP FAR PASCAL CreateMonochromeBitmap (HPAINT BitmapDC, 
                   WORD bmWidth, WORD bmHeight);


/* Local function prototypes */
void PrintAPIError (STATUS);


int main (int argc, char *argv[])
/************************************************************************

    FUNCTION:   main

    PURPOSE:    Main routine.

************************************************************************* 
*
* Parameters:
*   argv[1] = Note ID of document to be rendered (this is an optional
*             parameter.)
*
*/

{
   STATUS error=NOERROR;
   NOTEID SourceNoteID;
   char * endP;
  

   if (error = NotesInitExtended (argc, argv))
   {
      printf("\n Unable to initialize Notes.\n");
      return (1);
   }

   
   if (argc > 1)
      SourceNoteID = strtoul(argv[1],&endP,16);
   else
      SourceNoteID = 0;


   /*   Open the source database */

   if (error = NSFDbOpen(SOURCEDB, &hSourceDB))
      {
      printf("Cannot open source database %s: %e\n", SOURCEDB, error);
      goto exit0;
      }


   /*   Open the output database */

   if (error = NSFDbOpen(TARGETDB, &hTargetDB))
      {
      printf("Cannot open target database %s: %e\n", TARGETDB, error);
      goto exit1;
      }

   /*   If a NoteID was specified in the parameter, process the specified
        document;  otherwise, process all the documents.     */

   if (SourceNoteID)
      error = FaxDocument(SourceNoteID);
   else
   {
      error = NSFSearch(hSourceDB,
              NULLHANDLE,               /* Formula of @ALL */
              NULL,                     /* View title */
              0,                        /* Flags */
              NOTE_CLASS_DATA,          /* Note class mask */
              NULL,                     /* Starting search time */
              FaxEnumerationCallback,   /* Callback procedure */
              &hSourceDB,               /* Callback procedure argument */
              NULL);                    /* Ending search time */
   }
   goto exit2;


exit2:
   NSFDbClose(hTargetDB);

exit1:
   NSFDbClose(hSourceDB);

exit0:
   if (error)
      PrintAPIError(error);
   NotesTerm();
   return(error);
}


STATUS LNCALLBACK FaxEnumerationCallback (void *hSourceDB,
                  SEARCH_MATCH *pSearchMatch, ITEM_TABLE *pViewBuffer)

/*   FaxEnumerationCallback - Gets called for every note in the source 
*    database
*
* Inputs:
*
*   pSearchMatch = Address of SEARCH_MATCH
*   pViewBuffer = Address of summary buffer for match
*
* Outputs:
*
*   (Routine) = Error status
*/

   {
   return FaxDocument(pSearchMatch->ID.NoteID);
   }


STATUS near PASCAL FaxDocument (NOTEID SourceNoteID)

/*   FaxDocument - Test the FAX APIs on a single document
*
* Inputs:
*
*   SourceNoteID = NoteID of document to be FAXed.
*
* Outputs:
*
*   (Routine) = Error status
*/

   {
   STATUS     error=NOERROR;
   NOTEHANDLE hSourceNote;
   HBITMAP    hPrevBitmap;
   HBRUSH     hBrush, hPrevBrush;
   RECT       Rect;
   char       Buffer[256];

   /*   Create the target document to which the bitmaps will 
        be appended */

   if (error = NSFNoteCreate(hTargetDB, &hTargetNote))
      return(error);

   /*   Initialize the device parameters */

   memset(&EPBContext, 0, sizeof(EPBContext));

   /*   Set up the resolution parameters */ 

   EPBContext.cxPaperTwips = (DWORD) (WIDTH * ONEINCH);
   EPBContext.cyPaperTwips = (DWORD) (HEIGHT * ONEINCH);
   EPBContext.BitmapSize.width = (DWORD) (WIDTH * DPI);
   EPBContext.BitmapSize.height = (DWORD) (HEIGHT * DPI);

   /*   Set up pointer to callback routine */

   EPBContext.Proc = (EPBPROC) ProcessBitmap;

   /*   Creates a memory device context compatible with the
        application's current screen.  */  

   EPBContext.hPaint = CreateCompatibleDC(NULL);

   if (!EPBContext.hPaint)
      {
      printf("Cannot create memory DC\n");
      NSFNoteClose(hTargetNote);
      return(NOERROR);  
      }

   /*  Create a bitmap  */

   EPBContext.hBitmap = CreateMonochromeBitmap(EPBContext.hPaint,
                           EPBContext.BitmapSize.width,
                           EPBContext.BitmapSize.height);


   /*  Preset the bitmap to BLACK with a 10-pixel border and set the
       subset paint rectangle so that we can test to see that printing
       to a subset rect is working. */

   hPrevBitmap = (HBITMAP) SelectObject(EPBContext.hPaint, 
                                        (DHANDLE)EPBContext.hBitmap);

   Rect.left   = Rect.top = 0;
   Rect.right  = EPBContext.BitmapSize.width;
   Rect.bottom = EPBContext.BitmapSize.height;
   
   /* Create a Brush graphic object */
   hBrush = CreateSolidBrush(0x02000000L | (RGBVALUE_BLACK)); 
   hPrevBrush = SelectObject(EPBContext.hPaint, hBrush); 

   PatBlt(EPBContext.hPaint, (&Rect)->left,   /* Paints the given rectangle  */
               (&Rect)->top,                  /* using the brush that is     */                                 
               (&Rect)->right-(&Rect)->left,  /* currently selected into the */          
            (&Rect)->bottom-(&Rect)->top,  /* specified device context.   */
               PATCOPY); 

   SelectObject(EPBContext.hPaint, hPrevBrush); 
   
   SelectObject(EPBContext.hPaint, (DHANDLE)hPrevBitmap);

   EPBContext.PaintRect.top    = EPBContext.PaintRect.left   = 10;
   EPBContext.PaintRect.right  = EPBContext.BitmapSize.width - 10;
   EPBContext.PaintRect.bottom = EPBContext.BitmapSize.height- 10;

   /*   Print the note to the bitmaps */

   printf("Printing note %lX to bitmap...\n",SourceNoteID);

   error = EditorPrintNoteToBitmap(hSourceDB,
               SourceNoteID,
               &EPBContext);

   /*   Clean up. */

   DeleteObject(hBrush);
   DeleteObject(EPBContext.hBitmap);
   DeleteDC(EPBContext.hPaint);

   /*   Report errors from FAX rendering. */

   if (error)
      {
      printf("Error printing note to bitmap: \n");
      goto Done;
      }

   /*  Propagate a informational field from the source document
       to the new output document containing the bitmap, in order to
       easily correlate them. */

   if (!NSFNoteOpen(hSourceDB, SourceNoteID, 0, &hSourceNote))
      {
      if (NSFItemGetText(hSourceNote, "CompanyName", Buffer, sizeof(Buffer)-1))
         NSFItemSetText(hTargetNote, "CompanyName", Buffer, MAXWORD);
     else
         NSFItemSetText(hTargetNote, "CompanyName", COMPANYNAME, sizeof(COMPANYNAME));
      }
   

   if (error = NSFItemSetText(hTargetNote,"Form",
                FORMNAME,(WORD)strlen(FORMNAME)))
   {
       printf("Error creating Form item\n");
       goto Done;
   }

   /*   Write the bitmap to a note in the destination database. */

   if (error = NSFNoteUpdate(hTargetNote, 0))
   {
      printf("Error updating note: %e\n", error);
      goto Done;
   }

   /*   Close the note and database and exit. */

Done:
   NSFNoteClose(hTargetNote);

   return(error);
   }


HBITMAP FAR PASCAL CreateMonochromeBitmap (HPAINT BitmapDC, WORD bmWidth, WORD bmHeight)
/*
*   CreateMonochromeBitmap - create a monochrome bitmap of the
*            requested size in a OS independent fashion.
*
*   Inputs:
*      BitmapDC - DC for monochrome bitmap (unused in Windows)
*      bmWidth - x dimension of requested bitmap
*      bmHeight - y dimension of requested bitmap
*
*   Outputs:
*      (result) handle to monochrome bitmap (or NULLHANDLE if failure)
*/

   {
   HBITMAP hBitmap;

   hBitmap = CreateBitmap(bmWidth, bmHeight, (BYTE) 1,(BYTE) 1, NULL);

   return(hBitmap);
   }


BOOL LNCALLBACK ProcessBitmap (EPBCONTEXT *EPBContext, WORD PageNumber)

/*   Callback routine to process a bitmap
*
*   Inputs:
*      EPBCOntext
*      PageNumber - 1-based page number
*
*   Outputs:
*      (routine) = TRUE to continue processing, else FALSE to abort
*/

   {
   STATUS error=NOERROR;
   CDPABDEFINITION *pPAB;
   CDPABDEFINITION Pab;

   if (PageNumber > 1)     /* add a paragraph */
   {
      memset(&Pab, 0, sizeof(CDPABDEFINITION));
      Pab.Flags      = PABFLAG_PAGINATE_BEFORE | PABFLAG_PROPAGATE;
      Pab.LeftMargin = Pab.FirstLineLeftMargin = ONEINCH;
      Pab.PABID      = 0xffff;
      pPAB           = &Pab;
   }
   else
 
      pPAB = NULL;
 

   printf("Appending Page %d\n", PageNumber);

   if (error = EditorAppendBitmapToNote(EPBContext->hBitmap,
            (WORD)(WIDTH * ONEINCH), 
            (WORD)(HEIGHT * ONEINCH), 
            100,
            hTargetNote, 
            0, 
            "Body", 
            4, 
            pPAB))
   {
      printf("Can't append page %d bitmap: %e\n", PageNumber, error);
      PrintAPIError(error);
   }
   else
      printf("Page %d appended successfully\n", PageNumber);

   return (error ? FALSE : TRUE);
   }



/* This function prints the HCL C API for Notes/Domino error message
   associated with an error code. */

void PrintAPIError (STATUS api_error)

{
    STATUS  string_id = ERR(api_error);
    char    error_text[200];
    WORD    text_len;

    /* Get the message for this HCL C API for Notes/Domino error code
       from the resource string table. */

    text_len = OSLoadString (NULLHANDLE,
                             string_id,
                             error_text,
                             sizeof(error_text));

    /* Print it. */
    fprintf (stderr, "\n%s\n", error_text);

}

