#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

int scale_width(int w);
int scale_height(int h);

double distance(double xa, double ya, double xb, double yb);

SDL_Rect rect(int x, int y, int w, int h, _Bool autoscale);
SDL_Color color(int r, int g, int b, int a);

void centered_abs(SDL_Rect *rect);
void centered_ord(SDL_Rect *rect);
void centered_coord(SDL_Rect *rect);

int file_exists(const char *filename);

// Ajuste une vélocité donnée (en référence à 1920 pixels de largeur)
// Cette fonction renvoie une vitesse ajustée selon la largeur actuelle de l'écran,
double set_velocity_x(double var);

// Ajuste une vélocité donnée (en référence à 1080 pixels de hauteur)
// Similaire à SetVelocityX, mais adapté à la hauteur de l'écran.
double set_velocity_y(double var);

#endif // UTILS_H

