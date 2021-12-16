@echo off
setlocal
@echo Copying empty Name and Address book to Notes data directory...
COPY  ..\..\..\NOTEDATA\NA_TEST.NSF  C:\NOTES\DATA
prompt [$T]
cls

@echo Loading database with initial set of names...
@echo on
ADDNAMES  NA_TEST  50
@echo.
@echo.
@echo off

@echo Indexing the new names...
@echo on
INDEX  NA_TEST  Contacts
@echo.
@echo.
@echo off

@echo Performing mixed transactions with optimal indexing...
@echo on
MIXED_1  NA_TEST  40
@echo.
@echo.
@echo off

@echo Performing mixed transactions with worst-case indexing...
@echo on
MIXED_2  NA_TEST  40
@echo.
@echo.
echo Done
@echo off

endlocal
