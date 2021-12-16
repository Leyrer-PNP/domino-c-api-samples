Licensed Materials - Property of HCL
CIR9ZEN

(c) Copyright IBM Corporation   1996,   2013
(c) Copyright HCL Technologies   1996,   2021
All rights reserved.


PROGRAM
-------
nsfdb2recon    - Example HCL C API for Domino and Notes Program: 
Make sure that Domino can access NSFDB2 data for FullPath or rename the NSFDB2 database within the Domino server 

PURPOSE
-------
A utility function to make sure the infrastructure is in place such that Domino can access NSFDB2 data for FullPath.
This can also be used to effectively rename the NSFDB2 database within the Domino server by assigning a new FullPath 
if nsfid is provided.


ENVIRONMENTS
------------
Windows (32-bit)


FILES
-----
readme.txt     		- This file specifies what is needed to use this example.
nsfdb2recon.c	    - Demostrate the usage of DB2 releated APIS
mswin32.mak	   		- make file which will be used to build c file on windows 32-bit.


RUNNING this sample:
--------------------
-  Prior to run this sample, it is suggested that You should create a DB2 database, 
   And use the database name and domino server name as the sample's input parameters.
   please refer to the samples' code to get more information. 
   The third Parameter nsfID is is an unique number in DB2 catalog table for the DB_NAME,
   Please go to your DB2 control center and get this number.

-  Make sure that There is no problem with your DB2 enabled Domino setup or enablement.  
   To determine whether your domino server is DB2-enabled. Run command "show server" under
   domino server console. For more information , please refer to Notes help documents.
   
-  You must run this program in the server's partition. Or you will get error message says
   that "Domino Server is not DB2 enabled" 
   
   To achieve this, copy the executable to your Domino server's program directory.  
   This way, when it is invoked , it doesn't not need an =C:\path argument to point it to a 
   notes.ini rather it automatically finds and initializes to the server's notes.ini file.  
   This means that the program is running under the domino server's partition.

-  The assumption for the usage of the API NSFDB2ReconnectNotesDatabase would be that the 
   application would get rid of the old link file.The actual NSFDB2 name (e.g. mydb2nsf.nsf) 
   is a soft link and can really point to anything.
   
   So, during recovery, if you want to restore what was originally matt.nsf as bob.nsf, 
   you can simply call NSFDB2ReconnectNotesDatabase("bob.nsf"....., nsfid) and that function
   will do everything required in DB2 to change what was originally matt.nsf as bob.nsf.  
   However, this is not to be used as a rename function.  The intention is for recovery.  
   For example, the recovery application may have already taken matt.nsf offline or otherwise
   deleted it.
-  For the API NSFDB2ReconnectNotesDatabase, the first parameter FullPath  -  Relative to the 
   Domino server's data directory.  This does not have to be the same as the original path.
   
Verify the result:
--------------------
The sample runs successfully if you get result as below:


========init notes successfully!

========open the database:lewang/ibm!!target.nsf successfully

========Get DB2 info:DB2NSF_INFO_IS_DB2_BACKED,return value is 1

========Get DB2 info:DB2NSF_INFO_SCHEMA_NAME,return value is GRP6

========Get DB2 info:DB2NSF_INFO_TABLESPACE_NAME,return value is GRP6

========Get DB2 info:DB2NSF_INFO_TSID,return value is 12

========Get DB2 info:DB2NSF_INFO_USERSCHEMA_NAME,return value is TARGET

========Get DB2 info:DB2NSF_INFO_CLASS_DESC,return value is TARGET

========test api NSFDB2ReconnectNotesDatabase
        with domino data releative path and no nsfID set

execute  successfully

========test to recover the db, with the option nsfid specified

========NSFDB2 db recovered successfully!!

========execute NSFDBDelete successfully!!
 Terminating Notes...

========Program completed successfully.
