Licensed Materials - Property of HCL
CIR9ZEN
CIRA2EN

(c) Copyright IBM Corporation   1996,   2013
(c) Copyright HCL Technologies   1996,   2021
All rights reserved.


PROGRAM
-------
itemmisc - HCL C API for Domino and Notes Sample Program.


PURPOSE
-------
This program modifies an item value in a document and then copies the item to another document 
in the same view and renames it. It also checks that the documents in the view have a $Readers field.


ENVIRONMENTS
------------
Windows (32-bit)
Windows (64-bit)
Linux 64 bit


FILES
-----
readme.txt     - This file specifies what is needed to use this example.

itemmisc.c     - Main program.

mswin32.mak    - Makefile for Windows 32-bit.

mswin64.mak    - Makefile for Windows 64-bit.

linux64.mak    - Makefile for Linux 64 bit.


RUNNING setprivs:
-----------------
This sample program will run against the Domino database simple.nsf,
provided with the HCL C API for Domino and Notes toolkit.  You must
copy this file to the Domino data directory before running this program.

- Under Windows:
  itemmisc.exe <database> <view> 

- Under Linux:
  $/opt/hcl/domino/bin/itemmisc <database> <view> 

  If any of your arguments contain spaces, enclose them in quotes.


EXAMPLE
-------

The sample database, simple.nsf, has the following view:
    SimpleDataView

  itemmisc simple.nsf SimpleDataView 


If the program runs correctly, it should display:

 Note id [0x8f6] has reader field.
 Before Modifying Item's value: This is a line of simple text.
 Modified Item's value: This is a line of modified simple text.

 Note id [0x8fa] has no reader field.
 Copied doc [0x8f6] modified Item value to doc [0x8fa],
 Value of item is: [This is a line of modified simple text.] Item Name: [plainText].


CHECKING THE RESULTS
--------------------

 Note id [0x8f6] has reader field.
 Before Modifying Item's value: This is a line of simple text.
 Modified Item's value: This is a line of modified simple text.

 Note id [0x8fa] has no reader field.
 Copied doc [0x8f6] modified Item value to doc [0x8fa],
 Value of item is: [This is a line of modified simple text.] Item Name: [plainText].
