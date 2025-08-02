#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

int ScaleWidth(int w);
int ScaleHeight(int h);
int ScaleRefreshRate(int r);

int LCGRand(void);
int RandInt(int min, int max);

double Distance(double xa, double ya, double xb, double yb);

SDL_Rect Rect(int x, int y, int w, int h, _Bool autoscale);
SDL_Color Color(int r, int g, int b, int a);

void CenteredAbs(SDL_Rect *rect);
void CenteredOrd(SDL_Rect *rect);
void CenteredCoord(SDL_Rect *rect);

int FileExists(const char *filename);

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

