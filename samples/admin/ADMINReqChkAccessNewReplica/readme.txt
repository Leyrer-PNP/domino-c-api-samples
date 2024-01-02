Licensed Materials - Property of HCL
CIR9ZEN
CIRA0EN
CIRA2EN
CIRA1EN
 
(c) Copyright HCL Technologies   1996,   2023
All rights reserved.


PROGRAM
-------
ADMINReqChkAccessNewReplica - Sample program to show usage of ADMINReqChkAccessNewReplica.

ENVIRONMENTS
------------
Windows 64-bit
Linux 64 bit

FILES
-----
ADMINReqChkAccessNewReplica.c  - Main program.

mswin64.mak            - Makefile for Windows 64-bit.

linux64.mak            - Makefile for Linux 64 bit.

readme.txt             - This file specifies what is needed to use this example.


RUNNING ADMINReqChkAccessNewReplica:
-------------

The command line of ADMINReqChkAccessNewReplica is  -

    ADMINReqChkAccessNewReplica  <source server name> <source database name> <destination server name> <destination full path database name>

where:

    <source Server name> is the name of a server where the source database
                         resides.  
    <source file name> is the filename of the Domino source database.
   
    <destination Server name> is the name of a server where the destination database
                         resides.
    <destination full path database name> is the filename of the Domino destination database.

Note: For cluster setup please refer below link.
https://help.hcltechsw.com/domino/10.0.1/conf_creatingacluster_t.html#:~:text=Procedure%201%20From%20the%20Domino%20Administrator%2C%20click%20the,new%20cluster%2C%20and%20then%20click%20OK.%20More%20items
    