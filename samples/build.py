#!/usr/bin/env python
import platform
import sys
import os
import re
import datetime
import os.path
import glob
import getpass
import subprocess
import telnetlib
from collections import OrderedDict
from collections import namedtuple

"""
This program is used to build the sample program .
Following are the program logic,
1. Identify the OS and system architecture
2. Identify the make file to execute in the platform
3. If any existing make.err or make.out file present under sample
   directory, then take a backup of that with current date and time(dd_mm_yy_H_M)
4. For all the directories and sub-directories, call compilation procedure
5. On each directory trigger corresponding makefile and redirect the output 
   to make.err (all errors) and make.out (all logs)
6. Consolidate the compilation outputs under sample directory
7. Generate the report at the end of make.out file
8. Transfer the make.out & make.err to the jenkins machine (machine details passed as arguments)

"""


#Global varialbe to store make file name
MAKE_ERR = "make.err"
MAKE = "make"
MAKE_OUT = "make.out"
OUT = "out"
ERR = "err"
WIN_REMOTE_DIR = "C:\\tmp\\"
REMOTE_DIR = "/tmp"
COLUMN_LEN=54
UNIX_DIR_DIV = "/"
WIN_DIR_DIV = "\\"
SUCCESS = "Success"
FAIL = "Fail"

#No. of day older files to purge
PURGEDAYS = 10

#Global DataType
ReportDatTuple = namedtuple("ReportDatTuple", "total succ fail")
ReportCleanupTuple = namedtuple("ReportCleanupTuple", "exe obj link")

"""
To store the build execution status as a report

"""
class ReportStats:
    def __init__(self):
        self.mkCount = 0
        self.mkSucc = 0
        self.mkFail = 0
        self.filesList = OrderedDict()
        self.cleanupList = OrderedDict()

    def incMkCount(self):
        self.mkCount += 1
    def incMkSucc(self):
        self.mkSucc += 1
    def incMkFail(self):
        self.mkFail += 1
    def setCleanupList(self, dirName, fileExt, cnt):
        if (fileExt == "exe"):
            if (dirName in self.cleanupList.keys()):
               tmp = self.cleanupList[dirName].exe
               tmp += cnt
               self.cleanupList[dirName] = self.cleanupList[dirName]._replace(exe = tmp)
            else: 
               report = ReportCleanupTuple(exe=0, obj=0, link=0) 
               self.cleanupList[dirName]=report
        elif (fileExt == "obj"):
             if (dirName in self.cleanupList.keys()):
                tmp = self.cleanupList[dirName].obj
                tmp += cnt
                self.cleanupList[dirName] = self.cleanupList[dirName]._replace(obj = tmp)
             else: 
                report = ReportCleanupTuple(exe=0, obj=0, link=0) 
                self.cleanupList[dirName]=report
        elif (fileExt == "lnk"):
             if (dirName in self.cleanupList.keys()):
                tmp = self.cleanupList[dirName].link
                tmp += cnt
                self.cleanupList[dirName] = self.cleanupList[dirName]._replace(link = tmp)
             else: 
                report = ReportCleanupTuple(exe=0, obj=0, link=0) 
                self.cleanupList[dirName]=report

    def getMkCount(self):
        return self.mkCount
    def getMkSucc(self):
        return self.mkSucc
    def getMkFail(self):
        return self.mkFail
    def getMkFileList(self):
        return self.filesList

    def fileIndendation(self, fp, cLen, key, cStr):
        """
        This function is used to add indendation 
        """
        try:
           fp.write("|"+str(key))
           i = 0
           while (i < cLen) :
                 fp.write(" ")
                 i = i + 1
           fp.write(cStr+"|\n")
        except IOError as e:
            print("IO error on file write", format(e))
            raise
        except:    
            print("\n Unknown Error", sys.exec_info()[0])
            raise

    def addMkFile2Report(self, fileName, code):
        """
        This method to add filename and success/failure status in a map
        """
        if code == 0 :
           self.incMkSucc()
           self.filesList[fileName]=SUCCESS
        else:              
           self.incMkFail()
           self.filesList[fileName]=FAIL
 
    def cleanupSummary(self):
        """
        Building Report stats 
        """
        fp = None
        try:
            fp = open("./"+MAKE_OUT, "a+")
            fp.write("\n\n|------------------------------------------------------|")
            fp.write("\n|              Cleanup Report                          |")
            fp.write("\n|------------------------------------------------------|")
            fp.write("\n")
            lnk = 0
            obj = exe = 0
            for key in self.cleanupList:
                tmpTuple = self.cleanupList[key]
                lnk += tmpTuple.link
                exe += tmpTuple.exe
                obj += tmpTuple.obj
            #print(lnk, exe, obj)
            outString="No. of Exes deleted:"
            outValue=str(exe)
            cLen=54-len(outString)-len(outValue)
            self.fileIndendation(fp, cLen, outString, outValue)
            outString="No. of Object files deleted:"
            outValue=str(obj)
            cLen=54-len(outString)-len(outValue)
            self.fileIndendation(fp, cLen, outString, outValue)
            outString="No. of Symbolic Link Deleted:"
            outValue=str(lnk)
            cLen=54-len(outString)-len(outValue)
            self.fileIndendation(fp, cLen, outString, outValue)
            fp.write("|------------------------------------------------------|")

        except IOError as e:
            print("Error in file handling", format(e))
            raise
        except:
            print("\n Unknown Error", sys.exec_info()[0])
            raise
        finally:
            fp.close()

    def populateReport(self, dirDiv):
        """
        Method populates the report at the end of the make.out file
        """
        fp = None
        try:
            fp = open("./"+MAKE_OUT, "a+")

            """
            Build Summary 
            """
            fp.write("\n-----------------------------------------")
            fp.write("\n|        Build Summary                  |")
            fp.write("\n-----------------------------------------")
            fp.write("\n")
            filesList = self.getMkFileList()
            fp.write("|------------------------------------------------------|\n")
            #These logic is to display the folder wise status in tabular form
            baseDirMap = OrderedDict()
            flag = 0
            pbkey = ""
            for key in self.filesList:
                kLen=len(key)
                vLen=len(self.filesList[key])
                st = key.find(dirDiv) + 1
                end = key.find(dirDiv, st)
                bkey = key[st:end]
                if bkey in baseDirMap.keys():
                   #Fill the report tuple for generating report 
                   if (self.filesList[key] == SUCCESS):
                       tmp = baseDirMap[bkey].succ
                       tmp += 1
                       baseDirMap[bkey] = baseDirMap[bkey]._replace(succ = tmp)
                   else:
                       tmp = baseDirMap[bkey].fail
                       tmp += 1
                       baseDirMap[bkey] = baseDirMap[bkey]._replace(fail = tmp)
                   tmp = baseDirMap[bkey].total
                   tmp += 1
                   baseDirMap[bkey] = baseDirMap[bkey]._replace(total = tmp)
                else:
                    tmpF = tmpS = 0
                    if (self.filesList[key] == SUCCESS):
                       tmpS = 1
                    else:
                       tmpF = 1
                    report = ReportDatTuple(total=1, succ=tmpS, fail=tmpF)   
                    baseDirMap[bkey] = report
                    if flag == 1 :
                       tmpStr="Succs:" + str(baseDirMap[pbkey].succ) + " Fail:" + \
                       str(baseDirMap[pbkey].fail) + " Total:" + str(baseDirMap[pbkey].total) 
                       cLen=COLUMN_LEN-len(str(pbkey))-len(tmpStr)
                       fp.write("|------------------------------------------------------|\n")
                       self.fileIndendation(fp, cLen, pbkey, tmpStr)
                       fp.write("|------------------------------------------------------|\n")
                       fp.write("|------------------------------------------------------|\n")
                       pbkey = bkey
                    else:
                        pbkey=bkey
                    flag = 1
                cLen=COLUMN_LEN-kLen-vLen
                self.fileIndendation(fp, cLen, key, filesList[key])
                fp.write("|------------------------------------------------------|\n")
            if pbkey in baseDirMap.keys():    
               tmpStr="Succs:" + str(baseDirMap[pbkey].succ) + " Fail:" + \
               str(baseDirMap[pbkey].fail) + " Total:" + str(baseDirMap[pbkey].total) 
               cLen=COLUMN_LEN-len(str(pbkey))-len(tmpStr)
               fp.write("|------------------------------------------------------|\n")
               self.fileIndendation(fp, cLen, pbkey, tmpStr)
               fp.write("|------------------------------------------------------|\n")
               fp.write("|------------------------------------------------------|\n")

            """
            Building Report stats 
            """
            fp.write("\n\n|------------------------------------------------------|")
            fp.write("\n|              Build Report                            |")
            fp.write("\n|------------------------------------------------------|")
            fp.write("\n")
            outString="No. of build/make performed:"
            outValue=str(self.getMkCount())
            cLen=54-len(outString)-len(outValue)
            self.fileIndendation(fp, cLen, outString, outValue)
            outString="No. of build/make Success:"
            outValue=str(self.getMkSucc())
            cLen=54-len(outString)-len(outValue)
            self.fileIndendation(fp, cLen, outString, outValue)
            outString="No. of build/make Fail:"
            outValue=str(self.getMkFail())
            cLen=54-len(outString)-len(outValue)
            self.fileIndendation(fp, cLen, outString, outValue)
            fp.write("|------------------------------------------------------|")
            print("Build Report success")
        except OSError as err:
               print("\nError in file writing : " . format(err))
        except:
               print("\nUnknown Error:" . sys.exec_info()[0])
        finally:
               if (not fp.closed):
                  fp.close()



"""
Class is for user defined exception

"""
class EnvError(Exception):
      """Base class for other exceptions"""
      def __init__(self, message):
          self.msg = message
      # __str__ is to print() the value 
      def __str__(self): 
          return(repr(self.msg)) 

"""
Class that is used to telnet and continously 

"""
class StreamFile:

    def __init__(self, ip, user):
        self.host = ip
        self.user = user
        self.password = ''
        if (ip != ' ' and user != ''):
           self.password = getpass.getpass()

    def transferReport(self, remoteDir, fileName):
         
        #Collect all the make output to local machine

        try:
            if (self.host == '' or self.user == ''):
               print("***********************************")
               print("Its a local run. No log transfer   ")
               print("***********************************")
               return
        except Exception as e:  #Exception handling
             print(str(e))
             raise


"""
This method is to delete all bins and obj files in all sample dir
"""
def navigateAndClean(dirName, pattern, destPath, delFlag, osName):
    fileDeleted=0
    file_path=""
    cmdStr=""
    reg = re.compile(pattern)
    for filename in os.listdir(dirName):
        if (re.match(reg,filename.lower())):
           #print("\n match", filename)
           try:
              if (delFlag == 0):
                 file_path = os.path.join(dirName, filename).replace(' ', "\\ ")
              else:
                  if (destPath is not None):
                     file_path = os.path.join(destPath, os.path.splitext(filename)[0])
                  else:
                     file_path = os.path.join(dirName, os.path.splitext(filename)[0])
                  #print("file:",file_path)
              if (os.path.exists(file_path)):
                 if (osName != "mswin"):
                    cmdStr = " sudo rm -f " + file_path
                 else:
                    cmdStr = " del /f " + file_path
                 #print(cmdStr)
                 process=subprocess.Popen(cmdStr, stdin=None, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
                 stdOut, stdErr = process.communicate()
                 if (process.returncode):
                     #print("Cmd output:",stdOut, "Cmd Error:", stdErr)
                     raise Exception(stdErr)
                 fileDeleted += 1
           except:
                 print("File deletion Error:",  sys.exc_info()[0])
                 raise
    return fileDeleted


"""
This method is to delete all bins and obj files in all sample dir
"""
def cleanupBin(osName, dirDiv, makeFileName, env, reportStats):
    cwd=os.getcwd()
    print("cwd:", cwd)
    try:
       for root, dirs, files in os.walk(cwd, topdown=True):
           baseDir = os.path.basename(root)
           for dirName in dirs:
               print("\n ", root+dirDiv+dirName+dirDiv+makeFileName, dirName)
               path=root+dirDiv+dirName+dirDiv
               if (os.path.exists(path+makeFileName)): 
              
                  #Apped current working dir to replace
                  print("\nPath for deletion:", path)
                  if (osName != "mswin"):
                     cnt = navigateAndClean(path, r'\w+\.o$', None, 0, osName)
                     reportStats.setCleanupList(baseDir, "obj", cnt) 
                 
                     # To delete .so files if any
                     cnt = navigateAndClean(path, r'\w+\.so$', None, 0, osName)
                     reportStats.setCleanupList(baseDir, "obj", cnt) 
         
                     # To delete binary file
                     cnt = navigateAndClean(path, r'\w+\.c$', None, 1, osName)
                     reportStats.setCleanupList(baseDir, "exe", cnt)
                 
                     #Delete copied binary
                     cnt = navigateAndClean(path, r'\w+\.c$', os.path.join(env+"/notes/latest/", osName), 1, osName)
                     reportStats.setCleanupList(baseDir, "exe", cnt)
                  
                     #Delete link binary
                     #print("Env:", env)
                     cnt = navigateAndClean(path, r'\w+\.c$', os.path.join(env,"bin"), 1, osName)
                     reportStats.setCleanupList(baseDir, "lnk", cnt)

                  else:
                      #print("\nOS is windows", baseDir)
                      cnt = navigateAndClean(path, r'\w+\.obj$', None, 0, osName)
                      reportStats.setCleanupList(baseDir, "obj", cnt) 
                      cnt = navigateAndClean(path, r'\w+\.exe$', None, 0, osName)
                      reportStats.setCleanupList(baseDir, "exe", cnt) 
                      ncnt = navigateAndClean(path, r'\w+\.dll$', None, 0, osName)
                      reportStats.setCleanupList(baseDir, "obj", cnt) 
                      navigateAndClean(path, r'\w+\.pdb$', None, 0, osName)
    except FileNotFoundError:
           print("\n Skipping file not found")
    except:
           print("\n Unknown Error", sys.exc_info()[0])
           raise
    #sys.exit(1)



"""
Class that performs the build process 

"""
class BuildSample:
    def __init__(self, ip, user, cleanFlag, arch):
        self.osName=""
        self.mArch=""
        self.makeFileName=""
        self.ReportStats = ReportStats()
        self.StreamFile = StreamFile(ip, user)
        self.dirFormat=UNIX_DIR_DIV
        self.cleanup=cleanFlag
        self.lotusEnv= ""
        if (arch != '0'):
           self.mArch=arch

    def getEnv(self):
        self.lotusEnv=os.environ.get('LOTUS')
    
        if self.lotusEnv is None:
           raise(EnvError("Env variable is not set properly"))

    def populateOSName(self):
        """
        This is ued to get the current machine details

        """
        if platform.system().lower() == "windows": 
           self.osName="mswin"
           self.dirFormat=WIN_DIR_DIV
        elif platform.system().lower() == "linux": 
            self.osName="linux"
        elif platform.system().lower() == "aix":
            self.osName="aix"
        elif platform.system().lower() == "mac":
            self.osName="macosx"


    def populateMkFileName(self):
        """
        This function is used to get the system details like machine architecture 
        and OS name. If also constructs the make file name according to the OS and its architecture
        
        """
        self.populateOSName()
        
        if (self.mArch == ""): 
           if (platform.architecture()[0].lower() == "64bit"): 
               self.mArch="64";
           else:
               self.mArch="32";

        if self.osName == "mswin":
           self.makeFileName=self.osName+self.mArch+".mak"
        else:
            if (self.mArch == "64"):
               self.makeFileName=self.osName+self.mArch+".mak"
            else:
               self.makeFileName=self.osName+".mak"

        try:
           if (self.osName != "mswin"):
              self.getEnv()
           if (self.cleanup == True):
              cleanupBin(self.osName, self.dirFormat, self.makeFileName, self.lotusEnv, self.ReportStats)
           else:
               print("\n-----------------------------------------")
               print("\n Warning: Clean up is recomended         ")
               print("\n-----------------------------------------")
        except:
              raise

    def purgeBackupFiles(self):
        """
        This function is used to purge the backed-up files 
        older than the specified day

        """
        now = datetime.datetime.now() - datetime.timedelta(days=PURGEDAYS)
        dtString = now.strftime("%d_%m_%Y_%H_%M")
        fileListOut = glob.glob("*_*."+OUT )
        fileListErr = glob.glob("*_*."+ERR )

        # Iterate over the list of filepaths & remove each file.
        for fileName in fileListOut:
            dt = re.search("make_(.+?).out", fileName).group(1)
            d1 = datetime.datetime.strptime(dt, "%d_%m_%Y_%H_%M") 
            d2 = datetime.datetime.strptime(dtString, "%d_%m_%Y_%H_%M")
            #print("\nfile name:", fileName, " " , d1)
            try:
                if (d1 <= d2):
                    os.remove(fileName)
            except OSError as e:
                print("Error while deleting file", format(e))

        # Iterate over the list of filepaths & remove each file.
        for filePath in fileListErr:
            dt = re.search("make_(.+?).out", fileName).group(1)
            d1 = datetime.datetime.strptime(dt, "%d_%m_%Y_%H_%M") 
            d2 = datetime.datetime.strptime(dtString, "%d_%m_%Y_%H_%M")
            #print("\nfile name:", fileName, " " , d1)
            try:
                if (d1 <= d2):
                    os.remove(filePath)
            except OSError:
                print("Error while deleting file")
            except:
                print("Unknown Error", sys.exec_info()[0])
        print("\npurgeBackupFiles Completed")


    def backupOutAndErrFile(self):
        """ 
        This function is used to check the make.out and make.err file present under samples dir
        If files are present then backup the same with current date and time
        And clear the files for current run.

        """
        now = datetime.datetime.now()
        dtString = now.strftime("%d_%m_%Y_%H_%M")
        cwd=os.getcwd()
        errPath=os.path.join(cwd, MAKE_ERR)
        outPath=os.path.join(cwd, MAKE_OUT)
        try:
           if os.path.exists(errPath):
              os.rename(errPath, os.path.join(cwd,MAKE+"_"+ dtString + "." + ERR))
           if os.path.exists(outPath):
              os.rename(outPath, os.path.join(cwd,MAKE+"_"+ dtString + "." + OUT))
        except OSError as e:
               print("\nError in renaming the file ", format(e))
        except:
               print("\nUnknown Error:" . sys.exec_info()[0])
               raise
        print("\nbackupOutAndErrFile Completed")
        

    def callCompile(self):
        """
        This is used to get the directories under sample

        """
        self.backupOutAndErrFile()
        
        for root, dirs, files in os.walk(".", topdown=True):
            for name in files:
                if name == self.makeFileName:
                   self.triggerCompilation(root, self.makeFileName);
                        
    def writeLog2File(self, content, dstFilePath, statusCode, stdout, stderr):
        """
        Function used to format the compilation output & error

        """
        fwo = None
        fwe = None
        try:
           fwo = open(os.path.join(dstFilePath,MAKE_OUT), "a+")
           fwo.write(content)
           fwo.write(":\n")
           fwo.write(stdout.decode("UTF-8"))
           if statusCode == 0:
              fwo.write("\n`````````````Compilation Success```````````````````\n\n")
           else:
              fwo.write("\n`````````````Compilation failed````````````````````\n\n")

           fwe = open(os.path.join(dstFilePath,MAKE_ERR), "a+")
           fwe.write(content)
           fwe.write(":\n")
           fwe.write(stderr.decode("UTF-8"))
           if statusCode == 0:
              fwe.write("\n`````````````Compilation Success```````````````````\n\n")
           else:
              fwe.write("\n`````````````Compilation failed````````````````````\n\n")
           fwe.close()
           fwo.close()
        except OSError as err:
               print("\nError in file writing : " , err)
               raise
        except:
               print("\nUnknown Error:" , sys.exec_info()[0])
               raise

    def navigateAndCreate(self, dirPath, pattern):
        """
        This function is used to create a symbolic link for the executable on Linux platform

        """
        reg=re.compile(pattern)
        try:
           for filename in os.listdir(dirPath):
               if (re.match(reg,filename.lower())):
                  filename = os.path.splitext(filename)[0]
                  dir_path = os.path.join(dirPath + self.dirFormat , filename) 
                  if (os.path.exists(dir_path)):
                     cmdStr =  'sudo cp -f ' + dir_path + ' ' \
                     + os.path.join(os.path.join(self.lotusEnv+'/notes/latest/' , self.osName) , filename)
                     print(cmdStr)
                     process=subprocess.Popen(cmdStr, stdin=None, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
                     stdOut, stdErr = process.communicate()
                     if (process.returncode):
                         print("Cmd output:",stdOut, "Cmd Error:", stdErr)
                         raise Exception(stdErr)

                     cmdStr = "cd $LOTUS/bin/ ; " + "sudo ln -sf $LOTUS/bin/tools/startup " + filename
                     print(cmdStr)
                     process=subprocess.Popen(cmdStr, stdin=None, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True)
                     stdOut, stdErr = process.communicate()
                     
                     if (process.returncode):
                         raise Exception(stdErr)
        except:
              print("\n Unknown Error:", sys.exec_info()[0])
              raise


    def createSymLink(self, dirPath):
        """
        This function is used to create a symbolic link for the executable on Linux platform

        """
        try:
           self.navigateAndCreate(dirPath, '\w+\.c$')
           # Here we can add any files line .cpp .java etc   
        except OSError as err:
               print("\nError in file copy : " . err )
        except:
              print("\nUnknown Error:" . sys.exec_info()[0])
              raise

    def triggerCompilation(self, rootDir, makeFile):
        """
        Call make file to compile in the given directory
        
        """
        print("\nGoing to compile:", rootDir)
        self.ReportStats.incMkCount()
        cmdStr = ""
        if (self.osName == "mswin"):
           cmdStr = r'cd ' + os.path.abspath(rootDir ) + r' & nmake /f ' + makeFile + r' /a /d ' #make for windows machine
        else:
           cmdStr = r'cd ' + os.path.abspath(rootDir) + r' ; make -B -f ' + makeFile   #make for unix based machine

        try:
           process=subprocess.Popen(cmdStr, stdin=None, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True )
           stdOut, stdErr = process.communicate()
           if (process.returncode):
              print(cmdStr, " compilation Error" )

           self.ReportStats.addMkFile2Report(os.path.join(rootDir+self.dirFormat,makeFile), process.returncode)
           if (process.returncode == 0 and self.osName != "mswin") :
              self.createSymLink(rootDir)

           #Write compilation log in make.out under samples dir
           self.writeLog2File(os.path.join(rootDir,  makeFile), os.getcwd(), process.returncode, stdOut, stdErr)
        except OSError as err:
              print("\nError in compiling: " , err)
        except EnvError as err:
              print('\nEnv not set:', err.msg)
              sys.exit(1)
        except:
              print("\nUnknown Error", sys.exec_info()[0])
              pass

    
    def triggerFileTransfer(self):
        """
        This method is used to trigger the report generation and transfer the file
        """
        try:
            self.ReportStats.populateReport(self.dirFormat)
            self.ReportStats.cleanupSummary()
            print("\n transfer report to remote system ")
            #Send the Make_OUT & Make_ERR to the remote system 
            
            if (self.osName == "mswin"):
                self.StreamFile.transferReport(REMOTE_DIR, "./"+MAKE_OUT ) #using REMOTE_DIR, which is linux path for win use WIN_REMOTE_DIR
                self.StreamFile.transferReport(REMOTE_DIR, "./"+MAKE_ERR ) #change the dir accordingly
            else:
                self.StreamFile.transferReport(REMOTE_DIR, "./"+MAKE_OUT )
                self.StreamFile.transferReport(REMOTE_DIR, "./"+MAKE_ERR )
            
        except:
            print("\n Exception raised", sys.exec_info()[0])
            raise

"""
Print the program execution options

"""
def printOptions():
        print("\n----------------------------------------------------\n")
        print("Usage:\n")
        print(">python build.py <options> \n")
        print("Options: (optional)")
        print("-ip - Remote jenkins IP address \n")
        print("-u  - Remote jenkins user name \n")
        print("-c  - Perform binary and object file cleanup\n")
        print("-a  - Architecture for compilation 32 or 64 \n")
        print("\n")
        print("Example: python build.py -ip 127.0.0.1 -u admin -c -a 32 \n")
        print("----------------------------------------------------")
        sys.exit(1)

"""
Function to process command line arguments

"""
def processArgs():
    ip=''
    user=''
    cleanup=False
    arch='0'
    i = 1
    if (len(sys.argv) <= 1 or len(sys.argv) > 6):
        printOptions()
    while i < len(sys.argv):
        if (sys.argv[i] == "-ip"):
            if ((i+1) < len(sys.argv) and re.search("\d*\.\d*\.\d*\.\d*", sys.argv[i+1])):
                ip=sys.argv[i+1]
                i+=1
            else:
                print("\n Invalid IP address. Refer program exec options")
                printOptions()
        elif (i+1 < len(sys.argv) and sys.argv[i] == "-u"):
              if (sys.argv[i+1].startswith('-')):
                  printOptions()
              user=sys.argv[i+1]
              i+=1
        elif (sys.argv[i] == "-c"):
              cleanup=True
        elif ((i+1) < len(sys.argv) and sys.argv[i] == "-a"):
              if (sys.argv[i+1].startswith('-') or sys.argv[i+1].isnumeric() == False):
                  printOptions()
              arch=sys.argv[i+1]
              i+=1
        else:
             print("\nInvalid arguments options")
             printOptions()
        i+=1 
    return (ip, user, cleanup, arch)


"""
This main function to trigger the build process

"""
def main():

    ip, user, cleanup, arch = processArgs()

    try:
        build = BuildSample(ip, user, cleanup, arch)
    
        #Populate the make file name according to OS and architecture
        build.populateMkFileName()

        #Purge older than 10 days backup files
        build.purgeBackupFiles()

        #Compile the samples 
        build.callCompile()

        #Populate consolidated result
        build.triggerFileTransfer()
        print("Build process Completed!!")
    except:
        print("Build completed with error!!")
        sys.exit(1)
    
    sys.exit(0)

"""
Calling main function 

"""
main() 
