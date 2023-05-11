Licensed Materials - Property of HCL
CIR9ZEN

(c) Copyright IBM Corporation   1996,   2013
(c) Copyright HCL Technologies   1996,   2023
All rights reserved.


PROGRAM
-------
NSFDB2List - Example HCL C API for Domino and Notes Program: 
Provides a list of all NSFDB2 databases.

PURPOSE
-------
Provides a list of all NSFDB2 databases.  
An NSFDB2 database is a database that you can work with in DB2 and in Notes/Domino.


ENVIRONMENTS
------------
Windows (32-bit)


FILES
-----
readme.txt         - This file specifies what is needed to use this example.
NSFDB2List.C	   - Demostrate the usage of DB2 releated APIS
mswin32.mak	   - make file which will be used to build c file on windows 32-bit.


RUNNING this sample:
--------------------
-  Prior to run this sample, it is suggested that You should create a DB2 database, 
   And use the database name and domino server name as the sample's input parameters.
   please refer to the samples' code to get more information.

-  Make sure that There is no problem with your DB2 enabled Domino setup or enablement.  
   To determine whether your domino server is DB2-enabled. Run command "show server" under
   domino server console. For more information , please refer to Notes help documents.
   
-  You must run this program in the server's partition. Or you will get error message says
   that "Domino Server is not DB2 enabled" 
   
   To achieve this, copy the executable to your Domino server's program directory.  
   This way, when it is invoked , it doesn't not need an =C:\path argument to point it to a 
   notes.ini rather it automatically finds and initializes to the server's notes.ini file.  
   This means that the program is running under the domino server's partition.
   
Verify the result:
------------------

The sample runs successfully if you get result as below:

========begin the test
========begin to Construct the OSPathNet
========open DB lewang/ibm!!db6.nsf successfully
========Get DB Info:DB2NSF_INFO_TABLESPACE_NAME
 successfully
========execute NSFDB2ListNSFDB2Databases successfully!!

========now List ALL NSFDB2 DataBases

========We have 10 NSFDB2 Databases
        0: roaming\db2tester\bookmark.nsf
        1: roaming\db2tester\journal.nsf
        2: mail\db2test.nsf
        3: apitest\newnt140.nsf
        4: test.nsf
        5: roaming\tester2\bookmark.nsf
        6: roaming\tester2\journal.nsf
        7: mail\tester2.nsf
        8: target.nsf
        9: db6.nsf

========Program completed successfully.