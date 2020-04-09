
@echo off

DIR

::SETLOCAL
setlocal enabledelayedexpansion

::set errorlevel 0

if "%1"==""  (
@echo ERROR you must specify the configuration Debug or Release
exit /b -1 
)


SET exe_dir=bin\%1_%2
IF "%3"=="nightly" (
dir /b  %exe_dir%\*test-nightly.exe > testlist_%1.txt
) 
IF NOT "%3"=="nightly" (
dir /b  %exe_dir%\*test.exe > testlist_%1.txt 
)

if  NOT !errorlevel! == 0 ( 
    CALL  :abort %exe_dir%
    @exit /b -1
  )

CALL :header  %1 %2

IF NOT "%3"=="nightly" (
for /F "tokens=*" %%A in (testlist_%1.txt) do (
echo Running %%A in %1_%2 mode
%exe_dir%\%%A  %3 %4 %5 %6 %7 %8 %9
if  NOT !errorlevel! == 0 exit /b !errorlevel!
) )

IF "%3"=="nightly" (
for /F "tokens=*" %%A in (testlist_%1.txt) do (
echo Running %%A in %1_%2 mode
%exe_dir%\%%A   %4 %5 %6 %7 %8 %9
if  NOT !errorlevel! == 0 exit /b !errorlevel!
) )


CALL :footer %1 %2

goto eof;



:header
@echo ************************************************************
@echo ************************************************************
@echo ************************************************************
@echo **************  RUNNING TESTS IN %~1_%~2 MODE **************
@echo ************************************************************
@echo ************************************************************
@echo ************************************************************
goto eof;


:footer
@echo ************************************************************
@echo ************************************************************
@echo ************************************************************
@echo *********  FINISHED RUNNING TESTS IN %~1_%~2 MODE **********
@echo ************************************************************
@echo ************************************************************
@echo ************************************************************
goto eof;

:abort
@echo ************************************************************
@echo **********  ERROR, NO EXECUTABLES FOUND IN *****************
@echo ************************************************************                     
set RES=%%cd
@echo             %cd%\%~1 
@echo ************************************************************
@echo **********  ABORTING  **************************************
@echo ************************************************************

goto eof;

:eof