#!/bin/bash
# run.sh : Lance l'exécutable DroneWarfare

EXEC=./bin/DroneWarfare

if [ -x "$EXEC" ]; then
    "$EXEC"
else
    echo "Erreur : l'exécutable $EXEC n'existe pas ou n'est pas exécutable."
    exit 1
fi

