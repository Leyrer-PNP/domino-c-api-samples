Licensed Materials - Property of HCL
CIR9ZEN

(c) Copyright IBM Corporation   1996,   2013
(c) Copyright HCL Technologies   1996,   2023
All rights reserved.


PROGRAM
-------
NSFDB2FCopy  - Example HCL C API for Domino and Notes Program: 
Copy the NSFDB2 database specified and reconnects the copy of the DB2 data as an NSFDB2 database.

PURPOSE
-------
This function efficiently copies the NSFDB2 database specified by sourceNSF  and reconnects the 
copy of the DB2 data as an NSFDB2 database named  targetNSF . This function will only work locally 
on a Domino server supporting NSFDB2 databases.


ENVIRONMENTS
------------
Windows (32-bit)


FILES
-----
readme.txt     		- This file specifies what is needed to use this example.
NSFDB2FCopy.c	        - Demostrate the usage of DB2 releated APIS
mswin32.mak	        - make file which will be used to build c file on windows 32-bit.


RUNNING this sample:
--------------------
-  Prior to run this sample, it is suggested that You should create a DB2 database.    
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

-  You must enable "log retain" or "user exits" configuration in DB2, or you will get an error
   from the API NSFDB2FastCopy says that "A copy target cannot be provided when both log retain 
   and user exits are disabled."
   
Verify the result:
------------------

The sample runs successfully if you get result as below:

the fullpath_name is lewang/ibm!!db5.nsf

======notes init successfully
======Construct os netpath successfully
======open lewang/ibm!!test.nsf succeessfully

======the database is a NSFDB2 DB

begin to get the DB2 DB's information
======Get DB Info:DB2NSF_INFO_IS_DB2_BACKED successfully,return value is 1

======Get DB2 Info:DB2NSF_INFO_SCHEMA_NAME, return value is GRP1

======Get DB2 Info:DB2NSF_INFO_TABLESPACE_NAME, return value is GRP1

======Get DB Info:DB2NSF_INFO_TSID successfully,return value is 7

======Get DB2 Info:DB2NSF_INFO_USERSCHEMA_NAME, return value is TEST

======Get DB2 Info:DB2NSF_INFO_CLASS_DESC, return value is testclass??

======begin to get the DB2 server's information

======Get ServerInfo DB2NSF_SERVINFO_SERVER_DEFAULT_TYPE, return value is:7

======Get ServerInfo DB2NSF_SERVINFO_NSFDB2_CAPABLE, return value is:1

======Get ServerInfo DB2NSF_SERVINFO_DB2_DATABASE_NAME, return value is:1

======NSFDB2FastCopy successfully
======result verifies, succeed!