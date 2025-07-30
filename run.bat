@echo off
REM run.bat : Lance l'exécutable DroneWarfare

IF EXIST ".\bin\DroneWarfare.exe" (
    echo Lancement de DroneWarfare...
    .\bin\DroneWarfare.exe
) ELSE (
    echo Erreur : l'exécutable .\bin\DroneWarfare.exe n'existe pas ou n'est pas exécutable.
    exit /B 1
)

