# Richtext
These programs are in the richtext samples directory.

<details close markdown="block">
  <summary>
    Table of contents
  </summary>
  {: .text-delta }
1. TOC
{:toc}
</details>

## BIG_RICH (Windows, Linux)
Creates rich text fields from character text files up to five megabytes long using Compound Text functions

## CDFILE (Windows, AIX, Linux RedHat, IBM i)
Two programs to read and write a rich text (CD) file: CDFWRITE writes text to a CD file, and CDFREAD reads a CD file and stores it in the rich text field of a document

## DOCLINK (Windows, IBM i)
Adds a document link, a view link and a database link to a rich text field, using the low-level structures of a document, view and database link

## DUMPFONT (Windows, AIX, Linux RedHat, IBM i)
Locates and prints out the font tables in all data notes

## DYNAMIC (Windows, AIX, Linux RedHat, IBM i)
Same as WRICH, but uses dynamic allocation of the data structures
(This technique is useful if you do not know at compile time exactly what the rich text field will contain.)

## EASYRICH (Windows, AIX, Linux RedHat, IBM i)
Creates a rich text field using the simplest HCL C API functions
(If you want to write a rich text field, read this sample first.)

## FONTBL (Windows, AIX, IBM i, Linux RedHat)
Adds a font table to a document and writes some rich text to a rich text field, using fonts from the font table

## HISTORY (Windows)
Collects all response documents into the rich text field of the corresponding main document

## HOTSPOT (Windows, AIX, Linux RedHat, IBM i)
Creates a document with a popup, a button with formula, a bar, a URL, and a file attachment

## IMPORTER (Windows)
Imports graphics objects or word-processing files into a rich text field by calling a Notes import library

## JVAPPLET(Windows, Linux RedHat, IBM i)
Embed a Java applet in a rich text field in a document.

## LGIMPORT(Windows)
Imports large (>64K) graphics objects or word-processing files into a rich text field
(This sample calls a Notes import library and breaks the result into multiple rich text fields with the same name.)

## MKTABLE (Windows, AIX, Linux RedHat, IBM i)
Adds a table to a rich text field, using the low-level structures of a table

## OLE2\ATTACH (Windows)
Embeds an OLE 2 object which has been streamed to a structured storage file into a rich text field of a document

## OLE2\EXTRACT (Windows)
Extracts embedded OLE 2 objects to structured storage files

## OLE2\OLEASSOC (Windows)
Associate file(s) with an embedded OLE 2 object.

## OLE2\DISASSOC (Windows)
Disassociate file(s) from an embedded OLE 2 object.

## OLE2\GETASSOC (Windows)
List file(s) associated with an embedded OLE 2 object.

## RRICH (Windows, IBM i)
Reads a rich text field.

## WRICH (Windows, IBM i)
Creates a rich text field using low-level data structures
(For advanced features of rich text fields, you will need to use these structures. The structures are statically allocated in this example, which is useful as a tutorial or if the rich text item you want to create will always be same.)
