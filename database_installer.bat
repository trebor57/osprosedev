@echo off

REM ############################################
REM ## You can change here your own DB params ##
REM ############################################
REM MYSQL BIN PATH
set mysqlBinPath=C:\Program Files\MySQL\MySQL Server 5.0\bin

set user=root
set pass=
set db=prose
set host=localhost

REM ############################################

set mysqldumpPath="%mysqlBinPath%\mysqldump"
set mysqlPath="%mysqlBinPath%\mysql"

REM DONT TOUCH BELOW!
REM ############################################
REM ############################################

echo PLEASE EDIT THIS SCRIPT SO VALUES IN THE CONFIG SECTION MATCH YOUR DATABASE(S)
echo.
echo.
echo Making a backup of the original database.
%mysqldumpPath% --add-drop-table -h %host% -u %user% --password=%pass% %db% > prose_backup.sql
echo.
echo WARNING: this install will destroy all data in your
echo                        prose database.
echo          Choose upgrade (i) to install or q to quit.

:ask
set askprompt=x
set /p askprompt=DB install type: (i)  or (q) quit? 
if /i %askprompt%==i goto install
if /i %askprompt%==q goto end2
if /i %askprompt%==I goto install
if /i %askprompt%==Q goto end2
goto ask

:install
echo Installing database.
%mysqlPath% -h %host% -u %user% --password=%pass% -D %db% < prose.sql

:end
echo.
echo install complete.
pause
goto end3
:end2
echo not installed.
pause
end3