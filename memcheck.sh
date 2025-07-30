#!/bin/bash
# memcheck.sh : Lance Valgrind pour vérifier les fuites de mémoire de DroneWarfare

exec valgrind -s --leak-check=full --track-origins=yes ./bin/DroneWarfare "$@"

