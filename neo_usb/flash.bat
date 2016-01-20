@ECHO OFF
IF "%1"=="" GOTO ERR_HANDLE

REM=======================================================   
if exist %1 (
    ECHO [INFO] Check file [%1] found...
) else (
    ECHO [ERROR] File [%1] don't exist!!!
    GOTO ERR_HANDLE
)

echo [INFO] Replace \ to / for windows os
setlocal ENABLEDELAYEDEXPANSION
set word=/
set str=%1
set str=%str:\=!word!%
echo [INFO] Now the string is %str%

openocd -f board/stm32f4discovery.cfg  -c "init" -c "reset halt" -c "flash protect 0 0 11 off" -c "flash write_image erase %str%" -c "reset run" -c "exit"
GOTO End   
REM=======================================================   

:ERR_HANDLE
ECHO [ERROR] 
GOTO End

:End