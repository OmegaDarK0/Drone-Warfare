#include "score.h"
#include "globals.h"    // Pour la variable globale 'score'
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>   // Pour SDL_Log()

void RegisterScore(const char *player_name, const int difficulty) {
    FILE *file = fopen("score.txt", "a");
    if (!file) {
        SDL_Log("Erreur d'ouverture du fichier score.txt");
        return;
    }
    const time_t now = time(NULL);
    // Ecrit une ligne sous la forme : timestamp, nom_du_joueur, difficulté, score
    fprintf(file, "%ld,%s,%d,%d\n", now, player_name, difficulty, score);
    fclose(file);
}
