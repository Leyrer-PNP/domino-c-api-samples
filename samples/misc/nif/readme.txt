
(c) Copyright HCL Technologies   2018,   2023
All rights reserved.


PROGRAM
-------
nif.c - To test the NIF API's routine
NIFCollectionUpToDate, NIFIsNoteInView, NIFIsTimeVariantView and NIFReadEntriesExt.

PURPOSE
-----------
This program does the following checks
1. Checks whether the view is a time-varying view or any type of view that is rebuilt on each open, for example, a soft deleted view.
2. Checks whether the collection is up to date.
3. Checks whether the note is in the view.
4. Reads the entries and gets the last updated time of the NSF file.
           
ENVIRONMENTS
-----------------------
Windows (64-bit)
Linux 64 bit

FILES
-------
nif.c      - Main program.

mswin64.mak     - Makefile for Windows 64-bit.

linux64.mak    - Makefile for Linux 64 bit.

readme.txt    - This file specifies the requirements for this example.


RUNNING nif
--------------------
This sample program will run against the Domino database acls.nsf,
provided with the HCL C API for Domino and Notes toolkit.
You must copy this file to the Domino Data directory before 
running this program.

- Under Windows or Linux
  type the following at the system command prompt:
 
        nif <Database Name> <Time Variant View Name> <Normal View>
                 or
        nif
 
  where:
 
  <Database Name>,  <Time Variant View Name> and <View Name> are database,
  time variant view and normal view that is not time varying. If no command 
  line arguments are provided, the program will prompt for them.

Prerequisite: 
--------------------
Set env variable VIEW_REBUILD_DIR in notes.ini before executing. Restart the Domino server to reflect the new env variable.

Windows:
      Ex. VIEW_REBUILD_DIR=C:\TEMP
Linux:
      Ex. VIEW_REBUILD_DIR=/tmp

EXAMPLE:
-------------
	nif acls.nsf "simple view" "SimpleDataView"

Expected OUTPUT:
--------------------------

Testing NIFIsTimeVariantViewTest:
--------------------------------
[2EE4:0002-3F98] 22-06-2021 10:41:31   Unable to access view rebuild directory C:\notesapi1101\samples\misc\nif\TestDir. Using default temp directory instead.
In acls.nsf, the view "simple view" is a Time Variant View.
NIFIsTimeVariantViewTest was successful.


Testing NIFReadEntriesExtTest:
-------------------------------
Reading NSF file.
Empty buffer returned by NIFReadEntriesExt.
Last update to NSF file was on = 24-10-1992 22:04:36.
NIFReadEntriesExtTest was successful.

Creating note in NSF file:
----------------------------
Adding a note into NSF file.
Updating and closing note.

Testing NIFCollectionUpToDateTest:
---------------------------------

NIFCollection is not up to date; updating now.

NIFCollection is up to date.
NIFCollectionUpToDateTest was successful.

Testing NIFIsNoteInViewTest:
-----------------------------
ID  = 291 not found in collection.

Testing NIFIsNoteInViewTest:
-----------------------------
ID  = 8442 found in collection.

Testing NIFGetViewRebuildDir:
----------------------------
The rebuild directory is C:\notesapi1101\samples\misc\nif\TestDir.
NIFGetViewRebuildDir was successful.

Testing NIFReadEntriesExtTest after changing NSF file:
------------------------------------
Printing the entries after updating NSF file.

This is a line of simple text.    0.000987    15-01-1988      *    *.
This is another line of simple text.    836    04-07-1953 16:42:00      *    *.
Notes document created to test NIFReadEntriesExt    15    15-12-1997      *    *.
Last update to NSF file was on = 22-06-2021 10:41:31.
NIFReadEntriesExtTest was successful.

Program completed successfully.

