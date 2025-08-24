#include "menu.h"
#include "constants.h"
#include "entity.h"
#include "globals.h"
#include "utils.h"

#define LEVEL_MENU_COUNT 5
#define DIFFICULTY_MENU_COUNT 5

LevelMenuEntry level_menu[LEVEL_MENU_COUNT];
DifficultyMenuEntry difficulty_menu[DIFFICULTY_MENU_COUNT];

void SetLevelMenu(void){
    level_menu[0].area = rect(0, 0, 0, 0, 0);
    level_menu[0].music = res.level0_music;
    level_menu[0].background = res.level0_background;
    level_menu[0].level_id = 0;
    level_menu[1].area = rect(WIDTH / 5, HEIGHT / 5, WIDTH / 5, 3 * HEIGHT / 15, 0);
    level_menu[1].music = res.level1_music;
    level_menu[1].background = res.level1_background;
    level_menu[1].level_id = 1;
    level_menu[2].area = rect(3 * WIDTH / 5, HEIGHT / 5, WIDTH / 5, 3 * HEIGHT / 15, 0);
    level_menu[2].music = res.level2_music;
    level_menu[2].background = res.level2_background;
    level_menu[2].level_id = 2;
    level_menu[3].area = rect(WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, 3 * HEIGHT / 15, 0);
    level_menu[3].music = res.level3_music;
    level_menu[3].background = res.level3_background;
    level_menu[3].level_id = 3;
    level_menu[4].area = rect(3 * WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, 3 * HEIGHT / 15, 0);
    level_menu[4].music = res.level4_music;
    level_menu[4].background = res.level4_background;
    level_menu[4].level_id = 4;
}

void SetDifficultyMenu(void) {
    difficulty_menu[0].area = (SDL_Rect){2*WIDTH/5, HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[0].difficulty = 1;
    difficulty_menu[0].drone_config[0] = (EntityConfig){5, 45, 4, 1.5, 15, 60, 100};
    difficulty_menu[0].drone_config[1] = (EntityConfig){15, 60, 6, 0, 0, 0, 100};
    difficulty_menu[0].drone_config[2] = (EntityConfig){25, 45, 2, 1.5, 15, 50, 100};
    difficulty_menu[0].drone_config[3] = (EntityConfig){35, 60, 5, 2, 20, 100, 100};
    difficulty_menu[0].projectile_config[0] = (EntityConfig){.1, 2, 20, 0, 0, 0, 0};
    difficulty_menu[0].projectile_config[1] = (EntityConfig){.1, 5, 15, 0, 0, 0, 0};
    difficulty_menu[0].projectile_config[2] = (EntityConfig){.1, 2, 20, 0, 0, 0, 0};
    difficulty_menu[0].projectile_config[3] = (EntityConfig){.1, 5, 30, 0, 0, 0, 0};
    difficulty_menu[0].summon_delay = 6;
    difficulty_menu[1].area = (SDL_Rect){2*WIDTH/5, 2*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[1].difficulty = 2;
    difficulty_menu[1].drone_config[0] = (EntityConfig){10, 60, 5, 1.2, 12, 80, 100};
    difficulty_menu[1].drone_config[1] = (EntityConfig){20, 90, 7, 0, 0, 0, 95};
    difficulty_menu[1].drone_config[2] = (EntityConfig){30, 60, 2.5, 1.2, 12, 75, 100};
    difficulty_menu[1].drone_config[3] = (EntityConfig){40, 75, 6.25, 1.6, 16, 100, 100};
    difficulty_menu[1].projectile_config[0] = (EntityConfig){.1, 5, 20, 0, 0, 0, 0};
    difficulty_menu[1].projectile_config[1] = (EntityConfig){.1, 10, 15, 0, 0, 0, 0};
    difficulty_menu[1].projectile_config[2] = (EntityConfig){.1, 5, 20, 0, 0, 0, 0};
    difficulty_menu[1].projectile_config[3] = (EntityConfig){.1, 10, 30, 0, 0, 0, 0};
    difficulty_menu[1].summon_delay = 5;
    difficulty_menu[2].area = (SDL_Rect){2*WIDTH/5, 3*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[2].difficulty = 3;
    difficulty_menu[2].drone_config[0] = (EntityConfig){15, 75, 6, .9, 9, 90, 100};
    difficulty_menu[2].drone_config[1] = (EntityConfig){25, 120, 9, 0, 0, 0, 90};
    difficulty_menu[2].drone_config[2] = (EntityConfig){35, 75, 3, .9, 9, 80, 100};
    difficulty_menu[2].drone_config[3] = (EntityConfig){45, 90, 7.5, 1.2, 12, 100, 100};
    difficulty_menu[2].projectile_config[0] = (EntityConfig){.1, 10, 20, 0, 0, 0, 0};
    difficulty_menu[2].projectile_config[1] = (EntityConfig){.1, 15, 15, 0, 0, 0, 0};
    difficulty_menu[2].projectile_config[2] = (EntityConfig){.1, 10, 20, 0, 0, 0, 0};
    difficulty_menu[2].projectile_config[3] = (EntityConfig){.1, 20, 30, 0, 0, 0, 0};
    difficulty_menu[2].summon_delay = 4;
    difficulty_menu[3].area = (SDL_Rect){2*WIDTH/5, 4*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[3].difficulty = 4;
    difficulty_menu[3].drone_config[0] = (EntityConfig){20, 90, 7, .6, 6, 95, 100};
    difficulty_menu[3].drone_config[1] = (EntityConfig){30, 150, 11, 0, 0, 0, 75};
    difficulty_menu[3].drone_config[2] = (EntityConfig){40, 90, 3.5, .6, 6, 85, 100};
    difficulty_menu[3].drone_config[3] = (EntityConfig){60, 105, 8.75, .8, 8, 100, 100};
    difficulty_menu[3].projectile_config[0] = (EntityConfig){.1, 15, 20, 0, 0, 0, 0};
    difficulty_menu[3].projectile_config[1] = (EntityConfig){.1, 20, 15, 0, 0, 0, 0};
    difficulty_menu[3].projectile_config[2] = (EntityConfig){.1, 15, 20, 0, 0, 0, 0};
    difficulty_menu[3].projectile_config[3] = (EntityConfig){.1, 30, 30, 0, 0, 0, 0};
    difficulty_menu[3].summon_delay = 3;
    difficulty_menu[4].area = (SDL_Rect){2*WIDTH/5, 5*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[4].difficulty = 5;
    difficulty_menu[4].drone_config[0] = (EntityConfig){30, 105, 8, .3, 3, 100, 100};
    difficulty_menu[4].drone_config[1] = (EntityConfig){40, 180, 14, 0, 0, 0, 50};
    difficulty_menu[4].drone_config[2] = (EntityConfig){50, 105, 4, .3, 3, 90, 100};
    difficulty_menu[4].drone_config[3] = (EntityConfig){75, 120, 10, .4, 4, 100, 100};
    difficulty_menu[4].projectile_config[0] = (EntityConfig){.1, 20, 20, 0, 0, 0, 0};
    difficulty_menu[4].projectile_config[1] = (EntityConfig){.1, 25, 15, 0, 0, 0, 0};
    difficulty_menu[4].projectile_config[2] = (EntityConfig){.1, 20, 20, 0, 0, 0, 0};
    difficulty_menu[4].projectile_config[3] = (EntityConfig){.1, 40, 30, 0, 0, 0, 0};
    difficulty_menu[4].summon_delay = 2;
}

_Bool HandleLevelSelection(const SDL_Point cursor_pos) {
    if (cursor_pos.x == 0 && cursor_pos.y == 0) {
        easter_egg = 1;
        current_music = level_menu[0].music;
        current_background = level_menu[0].background;
        return 1;
    }
    for (int i = 1; i < LEVEL_MENU_COUNT; ++i) {
        if (SDL_PointInRect(&cursor_pos, &level_menu[i].area)) {
            current_music = level_menu[i].music;
            current_background = level_menu[i].background;
            return 1;
        }
    }
    return 0;
}

const SDL_Rect *GetHighlightRect(const SDL_Point cursor_pos) {
    for (int i = 1; i < 5; ++i) {
        if (SDL_PointInRect(&cursor_pos, &level_menu[i].area)) {
            return &level_menu[i].area;
        }
    }
    return NULL;
}

// Renvoie l’indice de la difficulté sélectionnée selon la position du curseur, -1 si aucune
int GetDifficultyAtPosition(const SDL_Point *pos) {
    for (int i = 0; i < 5; i++) {
        if (SDL_PointInRect(pos, &difficulty_menu[i].area)) {
            return i;
        }
    }
    return -1;
}

// Met à jour la difficulté si clic détecté, et applique la config correspondante
// Retourne SDL_TRUE si difficulté changée, SDL_FALSE sinon
_Bool HandleDifficultySelection(const SDL_Point *pos) {
    const int idx = GetDifficultyAtPosition(pos);
    if (idx >= 0) {
        // Récupère la config de difficulté
        const EntityConfig *drone_cfg = difficulty_menu[idx].drone_config;
        InitEntity(drone,
                   drone_cfg[0].health, drone_cfg[0].damage, drone_cfg[0].speed,
                   drone_cfg[0].rate_min, drone_cfg[0].rate_max, drone_cfg[0].precision, drone_cfg[0].stability);

        InitEntity(drone_explosive,
                   drone_cfg[1].health, drone_cfg[1].damage, drone_cfg[1].speed,
                   drone_cfg[1].rate_min, drone_cfg[1].rate_max, drone_cfg[1].precision, drone_cfg[1].stability);

        InitEntity(drone_nova,
                   drone_cfg[2].health, drone_cfg[2].damage, drone_cfg[2].speed,
                   drone_cfg[2].rate_min, drone_cfg[2].rate_max, drone_cfg[2].precision, drone_cfg[2].stability);

        InitEntity(drone_hunter,
                   drone_cfg[3].health, drone_cfg[3].damage, drone_cfg[3].speed,
                   drone_cfg[3].rate_min, drone_cfg[3].rate_max, drone_cfg[3].precision, drone_cfg[3].stability);
        const EntityConfig *projectile_cfg = difficulty_menu[idx].projectile_config;
        InitEntity(raw_plasma_orb,
                   projectile_cfg[0].health, projectile_cfg[0].damage, projectile_cfg[0].speed,
                   projectile_cfg[0].rate_min, projectile_cfg[0].rate_max, projectile_cfg[0].precision, projectile_cfg[0].stability);
        InitEntity(nova_orb,
                   projectile_cfg[1].health, projectile_cfg[1].damage, projectile_cfg[1].speed,
                   projectile_cfg[1].rate_min, projectile_cfg[1].rate_max, projectile_cfg[1].precision, projectile_cfg[1].stability);
        InitEntity(regular_bullet,
                   projectile_cfg[2].health, projectile_cfg[2].damage, projectile_cfg[2].speed,
                   projectile_cfg[2].rate_min, projectile_cfg[2].rate_max, projectile_cfg[2].precision, projectile_cfg[2].stability);
        InitEntity(sniper_bullet,
                   projectile_cfg[3].health, projectile_cfg[3].damage, projectile_cfg[3].speed,
                   projectile_cfg[3].rate_min, projectile_cfg[3].rate_max, projectile_cfg[3].precision, projectile_cfg[3].stability);
        SetAmmo(drone, regular_bullet);
        SetAmmo(drone_nova, nova_orb);
        SetAmmo(drone_hunter, sniper_bullet);
        summon_cooldown_max = difficulty_menu[idx].summon_delay;
        game_difficulty = difficulty_menu[idx].difficulty;
        return 1;
    }
    return 0;
}
