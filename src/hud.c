#include "hud.h"
#include <stdio.h>
#include "constants.h"
#include "engine.h"
#include "globals.h"
#include "utils.h"

//SDL_Texture *hitpoints_text;
//SDL_Texture *score_text;
Solid *hitpoints_info;
Solid *score_info;

SDL_Rect hitpoints_rect;
SDL_Rect hitpoints_max_rect;

void InitHUD() {
    hitpoints_rect = rect(WIDTH * 100 / 1920, 5, 100, 40, 1);
    hitpoints_max_rect = rect(0, 5, 0, 40, 1);
}

void DisplayHUD(const Entity *entity){
    if (!entity->effect){
        hitpoints_rect.w = scale_width((int)entity->hitpoints);
        hitpoints_max_rect.x = scale_width((int)entity->hitpoints) + hitpoints_rect.x;
        hitpoints_max_rect.w = scale_width((int)entity->hitpoints_max - (int)entity->hitpoints);
        DrawRect(hitpoints_rect, 0, 255, 0, 255);
        DrawRect(hitpoints_max_rect, 255, 0, 0, 255);
        DrawText(hitpoints_info, 0);
        DrawText(score_info, 0);
    }
}

void DisplayEffect(const Entity *entity){
    SDL_Rect R = rect(0, 0, 10, 10, 1);
    for (int i = 0; i < entity->effect * 10; i++) {
        R.x = randint(0, WIDTH - 10);
        R.y = randint(0, HEIGHT - 10);
        DrawRect(R, randint(0, 255), randint(0, 255), randint(0, 255), 255);
    }
}

void RenderScore() {
    char score_str[16];
    sprintf(score_str, "SCORE: %d", score);
    score_info->texture = LoadTextureFromText(font_large, score_str, WHITE);
}

void GainScore(const Entity *entity, const int difficulty){
    score += 5 * (int)pow(2, entity->category) * (entity->prestige + 1) * difficulty;
    RenderScore();
}
