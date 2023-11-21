Licensed Materials - Property of HCL
CIR9ZEN
CIRA0EN
CIRA2EN
CIRA1EN
 
(c) Copyright HCL Technologies   1996,   2023
All rights reserved.


PROGRAM
-------
ADMINReqDeleteInACLExt - Sample program to show usage of ADMINReqDeleteInACLExt.

ENVIRONMENTS
------------
Windows 64-bit
Linux 64 bit

FILES
-----
ADMINReqDeleteInACLExt.c  - Main program.

mswin64.mak            - Makefile for Windows 64-bit.

linux64.mak            - Makefile for Linux 64 bit.

readme.txt             - This file specifies what is needed to use this example.


RUNNING ADMINReqDeleteInACLExt:
-------------

The command line of ADMINReqDeleteInACLExt is  -

    ADMINReqDeleteInACLExt  <server name> <database filename>

where:

    <server name> is the name of a server where the database
                         resides.  

    <database filename> is the filename of the Domino database.The database
                         is Adminstartion request database "admin4.nsf"
                         which captures the "Delete in Access Control List" request.