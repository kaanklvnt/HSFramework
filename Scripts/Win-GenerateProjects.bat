@echo off

set ARGC=0
for %%x in (%*) do set /A ARGC+=1

if %ARGC% equ 0 (
	REM Update once we have a premake executable that supports VS2026.
	set STDVS=2022
) else if %ARGC% equ 1 (
	if "%1" neq "2017" (
		if "%1" neq "2019" (
			if "%1" neq "2022" (
				if "%1" neq "2026" (
					echo This script only supports Visual Studio versions 2017, 2019, 2022 and 2026 as of now. The version you have provided, %1, is not valid.
					goto TerminateDirect
				)
			)
		)
	)
	set STDVS=%1
) else (
	echo This script only allows up to one argument and that to be the year of the Visual Studio version to generate projects for. Giving zero arguments will generate for the latest VS version.
	goto TerminateDirect
)

set ARCHITECTURE=ARCHITECTURE_COULD_NOT_BE_RESOLVED
if %PROCESSOR_ARCHITECTURE% equ "" (
	echo The PROCESSOR_ARCHITECTURE environment variable doesn't exist or is set to nothing.
	goto TerminateDirect
)

if %PROCESSOR_ARCHITECTURE% equ AMD64 (
	set ARCHITECTURE=x64
) else if %PROCESSOR_ARCHITECTURE% equ ARM64 (
	set ARCHITECTURE=ARM64
) else (
	echo Your CPU architecture, %PROCESSOR_ARCHITECTURE%, is not supported by the this script.
	goto TerminateDirect
)

pushd %~dp0\..\

:EvalPath
set EXEC_PATH=.\ThirdParty\Premake\Binaries\Windows\%ARCHITECTURE%\premake5.exe
if not exist %EXEC_PATH% (
	if %ARCHITECTURE% neq x64 (
		echo The Premake executable for the %ARCHITECTURE% architecture couldn't be found. Falling back to the x64 one.
		set ARCHITECTURE=x64
		goto EvalPath
	)
	echo The Premake executable couldn't be located. Try pulling from the latest commit?
	goto TerminatePath
)

call %EXEC_PATH% vs%STDVS%

:TerminatePath
popd
:TerminateDirect
pause
exit /B
