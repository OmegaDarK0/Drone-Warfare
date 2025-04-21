#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

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

#undef EXTERN

#endif // GLOBALS_H

