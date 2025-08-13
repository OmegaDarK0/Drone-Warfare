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
    difficulty_menu[0].drone_config[0] = (EntityConfig){5, 2, 0.75, 4, 1.5, 15, 60, 100};
    difficulty_menu[0].drone_config[1] = (EntityConfig){15, 0, 1, 6, 0, 0, 0, 100};
    difficulty_menu[0].drone_config[2] = (EntityConfig){25, 5, 0.75, 2, 1.5, 15, 50, 100};
    difficulty_menu[0].drone_config[3] = (EntityConfig){35, 5, 1, 5, 2, 20, 100, 100};
    difficulty_menu[0].summon_delay = 6;
    difficulty_menu[1].area = (SDL_Rect){2*WIDTH/5, 2*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[1].difficulty = 2;
    difficulty_menu[1].drone_config[0] = (EntityConfig){10, 5, 1, 5, 1.2, 12, 80, 100};
    difficulty_menu[1].drone_config[1] = (EntityConfig){20, 0, 1.5, 7, 0, 0, 0, 95};
    difficulty_menu[1].drone_config[2] = (EntityConfig){30, 10, 1, 2.5, 1.2, 12, 75, 100};
    difficulty_menu[1].drone_config[3] = (EntityConfig){40, 10, 1.25, 6.25, 1.6, 16, 100, 100};
    difficulty_menu[1].summon_delay = 5;
    difficulty_menu[2].area = (SDL_Rect){2*WIDTH/5, 3*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[2].difficulty = 3;
    difficulty_menu[2].drone_config[0] = (EntityConfig){15, 10, 1.25, 6, .9, 9, 90, 100};
    difficulty_menu[2].drone_config[1] = (EntityConfig){25, 0, 2, 9, 0, 0, 0, 90};
    difficulty_menu[2].drone_config[2] = (EntityConfig){35, 15, 1.25, 3, .9, 9, 80, 100};
    difficulty_menu[2].drone_config[3] = (EntityConfig){45, 20, 1.5, 7.5, 1.2, 12, 100, 100};
    difficulty_menu[2].summon_delay = 4;
    difficulty_menu[3].area = (SDL_Rect){2*WIDTH/5, 4*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[3].difficulty = 4;
    difficulty_menu[3].drone_config[0] = (EntityConfig){20, 15, 1.5, 7, .6, 6, 95, 100};
    difficulty_menu[3].drone_config[1] = (EntityConfig){30, 0, 2.5, 11, 0, 0, 0, 75};
    difficulty_menu[3].drone_config[2] = (EntityConfig){40, 20, 1.5, 3.5, .6, 6, 85, 100};
    difficulty_menu[3].drone_config[3] = (EntityConfig){60, 30, 1.75, 8.75, .8, 8, 100, 100};
    difficulty_menu[3].summon_delay = 3;
    difficulty_menu[4].area = (SDL_Rect){2*WIDTH/5, 5*HEIGHT/6, WIDTH/5, HEIGHT/9};
    difficulty_menu[4].difficulty = 5;
    difficulty_menu[4].drone_config[0] = (EntityConfig){30, 20, 1.75, 8, .3, 3, 100, 100};
    difficulty_menu[4].drone_config[1] = (EntityConfig){40, 0, 3, 14, 0, 0, 0, 50};
    difficulty_menu[4].drone_config[2] = (EntityConfig){50, 25, 1.75, 4, .3, 3, 90, 100};
    difficulty_menu[4].drone_config[3] = (EntityConfig){75, 40, 2, 10, .4, 4, 100, 100};
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
        const EntityConfig *cfg = difficulty_menu[idx].drone_config;
        InitEntity(drone, 128, 128, DRONE,
                   cfg[0].HP, cfg[0].damage, cfg[0].m_damage, cfg[0].speed,
                   cfg[0].rate_min, cfg[0].rate_max, cfg[0].precision, cfg[0].stability);

        InitEntity(drone_sb, 128, 128, DRONE_SUICIDE_BOMBER,
                   cfg[1].HP, cfg[1].damage, cfg[1].m_damage, cfg[1].speed,
                   cfg[1].rate_min, cfg[1].rate_max, cfg[1].precision, cfg[1].stability);

        InitEntity(drone_nova, 141, 141, DRONE_NOVA,
                   cfg[2].HP, cfg[2].damage, cfg[2].m_damage, cfg[2].speed,
                   cfg[2].rate_min, cfg[2].rate_max, cfg[2].precision, cfg[2].stability);

        InitEntity(drone_hunter, 128, 128, DRONE_HUNTER,
                   cfg[3].HP, cfg[3].damage, cfg[3].m_damage, cfg[3].speed,
                   cfg[3].rate_min, cfg[3].rate_max, cfg[3].precision, cfg[3].stability);
        summon_cooldown_max = difficulty_menu[idx].summon_delay;
        game_difficulty = difficulty_menu[idx].difficulty;
        return 1;
    }
    return 0;
}
