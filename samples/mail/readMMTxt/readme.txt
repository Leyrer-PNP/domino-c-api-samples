Licensed Materials - Property of HCL
CIR9ZEN
CIRA2EN

(c) Copyright IBM Corporation   1996,   2013
(c) Copyright HCL Technologies   1996,   2023
All rights reserved.



PROGRAM
-------
readMMTxt - Get the text contents of all MIME messages in a NSF Database.

ENVIRONMENTS
------------
Windows (32-bit)
Windows (64-bit)
Linux   (64-bit)
IBM i

PURPOSE
-------
This program shows how to use the MIME functions to extract the text part from the MIME messages(Multipurpose 
Internet Mail Extensions) in a NSF database.

This program first scans all the notes in the specified Dabase. For those messages which have multi
MIME part, it extract the text parts of the messages, both the plain text and also the html text, and output them
to an output file. However,this programme doesn't scan all the part of the messages, and doesn't the expand the
attachments even they are plain text.


FILES
-----
readme.txt    - This file.

readMMTxt.c  - Main C program.

mswin32.mak     - Make file for Windows 32-bit.

mswin64.mak     - Make file for Windows 64-bit.

linux64.mak     - Make file for Linux 64-bit.

mk_readmtxt_i.clp     - Make file (CL Program) for IBM i using ILE C 
                       with the source code being an IFS file. 
                          
mk_readmtxt_p.clp     - Make file (CL Program) for IBM i using ILE C 
                       with the source code being in a source physical file. 

RUNNING readMMTxt
-------------------

- This sample provides a sample Database(MIMEMsg.nsf) in the notedata folder.
  If needed, copy it to you HCL Notes data directory.

- At the command-line the syntax for this sample is:

      readMMTxt 
         OR
      readMMTxt  <NSF DB>

EXAMPLE
-------

     readMMTxt  MIMEmsg.nsf
 
CHECKING THE RESULT
--------------------    
     
     After the sample returns successfully, there is an output in the console screen. 
