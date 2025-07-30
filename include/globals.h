#ifndef GLOBALS_H
#define GLOBALS_H

#include "structures.h"

#ifdef DEFINE_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN unsigned long int seed;
EXTERN int entity_count;
EXTERN int summon_delay;
EXTERN int summon_delay_max;
EXTERN int score;

EXTERN SDL_bool running;
EXTERN SDL_bool debug;
EXTERN SDL_Texture *debug_hitbox;
EXTERN SDL_Window *window;
EXTERN SDL_Renderer *renderer;

EXTERN int font_small;
EXTERN int font_medium;
EXTERN int font_large;
EXTERN int font_xxl;
EXTERN int font_jumbo;

// Déclaration de la variable globale ResourceManager
EXTERN ResourceManager res;

// Autres variables globales nécessaires
EXTERN _Bool easter_egg;
EXTERN SDL_bool key_up, key_down, key_left, key_right, key_mouse, key_pause, text_input;
EXTERN char player_name[9];

#undef EXTERN

#endif // GLOBALS_H
