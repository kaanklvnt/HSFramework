@echo off

call net.exe session >nul 2>&1
if %errorLevel% neq 0 (
	echo This script needs to be run with administrative privileges as it modifies the HSFW_PATH system environment variable.
	pause
	exit /B
)

pushd %~dp0\..\
setx /M HSFW_PATH %CD%
popd

pause
exit /B
