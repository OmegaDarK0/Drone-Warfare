#include "hud.h"
#include "constants.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "images.h"
#include "ui.h"
#include <stdio.h>
#include <math.h>

static Solid *hitpoints_info;
static Solid *score_info;

static SDL_Rect hitpoints_rect;
static SDL_Rect hitpoints_max_rect;

void ClearHUD(void){
    DestroySolid(hitpoints_info);
    DestroySolid(score_info);
}

void InitHUD(void) {
    hitpoints_rect = Rect(ScaleWidth(120), BORDER_OFFSET + 7, 100, 40, 1);
    hitpoints_max_rect = Rect(0, BORDER_OFFSET + 7, 0, 40, 1);
    hitpoints_info = CreateSolidText(Rect(BORDER_OFFSET, BORDER_OFFSET, 1, 1, 0), WHITE, "HP", font_large, WHITE);
    score_info = CreateSolidText(Rect(BORDER_OFFSET, HEIGHT - HEIGHT * 50 / 1080 - BORDER_OFFSET, 1, 1, 0), WHITE, "SCORE: 0", 45, WHITE);
    // Vous pouvez initialiser ici hitpoints_info et score_info, par exemple :
    // hitpoints_info = CreateSolid(hitpoints_rect, color(0, 255, 0, 255), "HP");
    // score_info = CreateSolidText(...);
}

void DisplayHUD(const Entity *entity) {
    if (entity->effect <= 0) {
        hitpoints_rect.w = ScaleWidth((int)entity->hitpoints);
        hitpoints_max_rect.x = ScaleWidth((int)entity->hitpoints) + hitpoints_rect.x;
        hitpoints_max_rect.w = ScaleWidth((int)(entity->hitpoints_max - entity->hitpoints));
        DrawRect(hitpoints_rect, 0, 255, 0, 255);
        DrawRect(hitpoints_max_rect, 255, 0, 0, 255);
        DrawText(hitpoints_info, 0);
        DrawText(score_info, 0);
    }
}

void DisplayEffect(const Entity *entity) {
    SDL_Rect R = Rect(0, 0, 10, 10, 1);
    for (int i = 0; i < entity->effect * 1000; i++) {
        R.x = RandInt(0, WIDTH - 10);
        R.y = RandInt(0, HEIGHT - 10);
        DrawRect(R, RandInt(0,255), RandInt(0,255), RandInt(0,255), 255);
    }
}

void RenderScore(void) {
    char score_str[16];
    sprintf(score_str, "SCORE: %d", player_score);
    score_info->texture = LoadTextureFromText(score_str, font_large, WHITE);
}

void GainScore(const Entity *entity, const int difficulty) {
    player_score += 5 * (int)pow(2, entity->type) * (entity->prestige + 1) * difficulty;
    RenderScore();
}

