@echo off

REM Check args

if "%1"=="" goto usage
if "%2"=="" goto usage
if "%3"=="" goto usage

echo Merging updates
cd utils

rem STEP 1: Execute merge-evemu-updates.bat
call merge-evemu-updates.bat

rem STEP 2: Execute merge-ofic-updates.bat
call merge-ofic-updates.bat

cd ..

echo Completed. Press any key to auto-import SQL files or close this program to do a manual import
pause

echo Creating temporary SQL file
rem STEP 3: Generate SQL file
echo -- EVEmu Setup File Generated > evemu_database.sql
echo source apo15-mysql5-v1.sql; >> evemu_database.sql
echo source ofic-updates.sql; >> evemu_database.sql
echo source evemu_static-dump.sql; >> evemu_database.sql
echo source evemu_dynamic-dump.sql; >> evemu_database.sql
echo source evemu-updates.sql; >> evemu_database.sql
echo source prime_db.sql; >> evemu_database.sql

echo Adding info to SQL server
rem STEP 4: Query all the files
mysql -u%1 -p%2 %3 < evemu_database.sql

echo Deleting temporary SQL file
del evemu_database.sql

echo Imported SQL files
goto done

:usage

echo.
echo Error in arguments supplied to batch. See usage info below:
echo.
echo evemu_setup_database.bat [username] [password] [db]
echo.

:done
pause