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

#endif /* UTILS_H */
