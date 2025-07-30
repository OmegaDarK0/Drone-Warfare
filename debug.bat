@echo off
REM debug.bat : Lance GDB pour déboguer DroneWarfare

IF EXIST ".\bin\DroneWarfare.exe" (
    echo Démarrage de GDB...
    gdb .\bin\DroneWarfare.exe %*
) ELSE (
    echo Erreur : l'exécutable .\bin\DroneWarfare.exe n'existe pas.
    exit /B 1
)

