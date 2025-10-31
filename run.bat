@echo off
cd /d "%~dp0"
set BINARY=bin\DroneWarfare.exe

if not exist "%BINARY%" (
    echo Error: binary "%BINARY%" not found.
    exit /b 1
)

echo Starting %BINARY%...
"%BINARY%" %*
