@echo off

cd ..

set INPUT=evemu-updates\*.sql
set TEMPF=evemu_updates_merge.tmp
set OUTPUT=evemu-updates.sql

if exist %OUTPUT% del %OUTPUT%
if exist %TEMPF% del %TEMPF%

type %INPUT% > %TEMPF%
move %TEMPF% %OUTPUT%

echo File %OUTPUT% has been successfully created.

cd utils