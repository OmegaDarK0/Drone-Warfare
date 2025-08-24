#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "structures.h"

// Gestion des déclarations/références externes
#ifdef DEFINE_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

/* =========================
   == États du jeu ==
   ========================= */
EXTERN GameState game_state;
EXTERN SDL_bool game_running;
EXTERN SDL_bool game_restart;
EXTERN SDL_bool game_debug;
EXTERN int game_difficulty;
EXTERN int player_score;

/* =========================
   == Entrées utilisateur ==
   ========================= */
EXTERN PlayerInfo player_info;
EXTERN SDL_bool key_up, key_down, key_left, key_right;
EXTERN SDL_bool key_mouse, key_pause, text_input;
EXTERN _Bool easter_egg;

/* =========================
   == Ressources ==
   ========================= */
EXTERN SDL_Window *window;
EXTERN SDL_Renderer *renderer;
EXTERN SDL_Texture *current_background;
EXTERN Mix_Music *current_music;
EXTERN ResourceManager res;

/* =========================
   == Polices ==
   ========================= */
EXTERN int font_small;
EXTERN int font_medium;
EXTERN int font_large;
EXTERN int font_xxl;
EXTERN int font_jumbo;

/* =========================
   == Entités du jeu ==
   ========================= */
EXTERN Entity *player;
EXTERN Entity *drone, *drone_explosive, *drone_nova, *drone_hunter;
EXTERN Entity *regular_bullet, *sniper_bullet, *plasma_orb, *raw_plasma_orb, *nova_orb;
EXTERN EntityList *team1, *team2;

/* =========================
   == UI / Solides ==
   ========================= */
EXTERN Solid *cursor;
EXTERN Solid *easy_button, *normal_button, *hard_button, *harder_button, *insane_button;
EXTERN Solid *restart_button, *game_over_title, *pause_title, *score_title;

/* =========================
   == Conteneurs de niveaux ==
   ========================= */
EXTERN SDL_Rect level1_container, level2_container;
EXTERN SDL_Rect level3_container, level4_container;

/* =========================
   == Autres ==
   ========================= */
EXTERN int entity_count;
EXTERN double summon_cooldown;
EXTERN double summon_cooldown_max;

EXTERN double DELTA_TIME;
EXTERN double SMOOTHED_DT;

#undef EXTERN
#endif // GLOBALS_H
