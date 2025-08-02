#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef DEFINE_CONSTANTS
  #define EXTERN
#else
  #define EXTERN extern
#endif

// Dimensions écran et taux de rafraîchissement
EXTERN int WIDTH;
EXTERN int HEIGHT;
EXTERN int REFRESH_RATE;

EXTERN int FPS;
EXTERN unsigned int FRAME_DELAY;
EXTERN double DELTA_TIME;

// Autres constantes
#define BORDER_OFFSET 5

// Couleurs SDL utilisées dans le projet
#include <SDL2/SDL.h>
EXTERN SDL_Rect SCREEN;
static const SDL_Color VOID = {0, 0, 0, 0};
static const SDL_Color BLACK = {0, 0, 0, 255};
static const SDL_Color WHITE = {255, 255, 255, 255};
static const SDL_Color RED   = {255, 0, 0, 255};
static const SDL_Color YELLOW= {255, 255, 0, 255};
static const SDL_Color GREEN = {0, 255, 0, 255};
static const SDL_Color CYAN  = {0, 255, 255, 255};
static const SDL_Color BLUE  = {0, 0, 255, 255};
static const SDL_Color MAGENTA = {255, 0, 255, 255};

#undef EXTERN

#endif // CONSTANTS_H
