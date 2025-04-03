#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>

extern unsigned long int seed;

extern int entity_count;
extern int summon_delay;
extern int summon_delay_max;
extern int score;

extern SDL_bool running;
extern SDL_bool debug;
extern SDL_Texture *debug_hitbox;
extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern int font_small;
extern int font_medium;
extern int font_large;
extern int font_xxl;
extern int font_jumbo;

#endif /* GLOBALS_H */
