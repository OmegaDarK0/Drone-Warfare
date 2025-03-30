#include "globals.h"

unsigned long int seed = 1;

int entity_count = 0;
int summon_delay = 200;
int summon_delay_max = 0;
int score = 0;

SDL_bool running = SDL_TRUE;
SDL_bool debug = SDL_FALSE;
SDL_Texture *debug_hitbox;
SDL_Window *window;
SDL_Renderer *renderer;
FT_Library ft_library;
FT_Face font_small;
FT_Face font_medium;
FT_Face font_large;
FT_Face font_xxl;
FT_Face font_jumbo;
