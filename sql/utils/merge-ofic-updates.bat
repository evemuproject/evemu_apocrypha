@echo off

cd ..

set INPUT=ofic-updates\*.sql
set TEMPF=ofic_updates_merge.tmp
set OUTPUT=ofic-updates.sql

if exist %OUTPUT% del %OUTPUT%
if exist %TEMPF% del %TEMPF%

type %INPUT% > %TEMPF%
move %TEMPF% %OUTPUT%

echo File %OUTPUT% has been successfully created.

cd utils