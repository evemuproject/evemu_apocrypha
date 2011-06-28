@ECHO off

REM CHECK ARGUMENTS FOR NON-BLANK VALUES:
if "%1"=="" goto usage
if "%2"=="" goto usage

cd .\utils\

rem STEP 1: Execute merge-evemu-updates.bat
call merge-evemu-updates.bat

rem STEP 2: Execute merge-ofic-updates.bat
call merge-ofic-updates.bat

cd ..

rem STEP 3: Apply evemu mysql updates, all of them
mysql -u %1 -p %2 < evemu_setup_database.sql
goto done


:usage

ECHO.
ECHO Error in arguments supplied to batch.  See usage info below:
ECHO.
ECHO prompt:> evemu_setupdatabase.bat <username> <evemu_database_name>
ECHO.
ECHO example:  > evemu_setup_database root evemu
ECHO.

:done
