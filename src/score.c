#include "score.h"
#include "globals.h"    // Pour la variable globale 'score'
#include <stdio.h>
#include <SDL2/SDL.h>   // Pour SDL_Log()

void RegisterScore(const char *player_name, const int difficulty) {
    FILE *file = fopen("score.txt", "a");
    if (!file) {
        SDL_Log("Erreur d'ouverture du fichier score.txt");
        return;
    }
    fprintf(file, "%s : %d", player_name, score);
    switch (difficulty) {
        case 1: fprintf(file, " | EASY\n"); break;
        case 2: fprintf(file, " | MEDIUM\n"); break;
        case 3: fprintf(file, " | HARD\n"); break;
        case 4: fprintf(file, " | HARDER\n"); break;
        case 5: fprintf(file, " | INSANE\n"); break;
        default: fprintf(file, "\n");
    }
    fclose(file);
}
