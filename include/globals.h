#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <freetype2/freetype/freetype.h>

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
extern FT_Library ft_library;
extern FT_Face font_small;
extern FT_Face font_medium;
extern FT_Face font_large;
extern FT_Face font_xxl;
extern FT_Face font_jumbo;

#endif /* GLOBALS_H */
