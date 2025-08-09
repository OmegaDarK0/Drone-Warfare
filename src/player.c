#include "player.h"
#include "globals.h"
#include "constants.h"
#include "entity.h"
#include "images.h"
#include "utils.h"

void UpdatePlayer(void) {
    const int vx = (int)key_right - (int)key_left;
    const int vy = (int)key_down - (int)key_up;
    player->velocity_x = set_velocity_x(vx);
    player->velocity_y = set_velocity_y(vy);

    if (key_mouse) {
        EntityShoot(player, player->projectiles, plasma_orb,
                    (float)cursor->rect.x, (float)cursor->rect.y, 0, 0);
    }
}

void RegisterScore(const char *player_name, const int difficulty) {
    FILE *file = fopen("score.txt", "a");
    if (!file) {
        SDL_Log("Erreur d'ouverture du fichier score.txt");
        return;
    }
    fprintf(file, "%s : %d", player_name, player_score);
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

void ResetPlayerInfo(PlayerInfo *player_info){
    player_info->name[0] = '\0';
    player_info->length = 0;
    player_info->title->texture = LoadTextureFromText("ENTER YOUR NAME", font_large, WHITE);
}
