#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

int scale_width(int w);
int scale_height(int h);
int scale_refresh_rate(int r);

int lcg_rand(void);
int randint(int min, int max);

double distance(double xa, double ya, double xb, double yb);

SDL_Rect rect(int x, int y, int w, int h, _Bool autoscale);
SDL_Color color(int r, int g, int b, int a);

void centered_abs(SDL_Rect *rect);
void centered_ord(SDL_Rect *rect);
void centered_coord(SDL_Rect *rect);

int file_exists(const char *filename);

// Convertit un temps en secondes en nombre de frames
int TimeToFrames(double seconds);

// Ajuste une vélocité donnée (en référence à 1920 pixels de largeur)
// Cette fonction renvoie une vitesse ajustée selon la largeur actuelle de l'écran,
// le taux de rafraîchissement et une base de 60 fps.
double SetVelocityX(double var);

// Ajuste une vélocité donnée (en référence à 1080 pixels de hauteur)
// Similaire à SetVelocityX, mais adapté à la hauteur de l'écran.
double SetVelocityY(double var);

#endif // UTILS_H

