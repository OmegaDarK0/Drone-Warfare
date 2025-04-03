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

int font_small = 15;
int font_medium = 25;
int font_large = 50;
int font_xxl = 100;
int font_jumbo = 200;
