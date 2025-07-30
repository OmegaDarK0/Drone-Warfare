@echo off
REM memcheck.bat : Lance Valgrind pour vérifier les fuites memoire de DroneWarfare
IF EXIST ".\bin\DroneWarfare.exe" (
    echo Lancement de Valgrind...
    valgrind -s --leak-check=full --track-origins=yes .\bin\DroneWarfare.exe %*
) ELSE (
    echo Erreur : l'exécutable .\bin\DroneWarfare.exe n'existe pas.
    exit /B 1
)

