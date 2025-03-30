#include "utils.h"
#include "globals.h"
#include "constants.h"
#include <stdlib.h>

int scale_width(const int w) {
    return WIDTH * w / 1920;
}

int scale_height(const int h) {
    return HEIGHT * h / 1080;
}

int scale_refresh_rate(const int r) {
    return REFRESH_RATE * r / 60;
}

int lcg_rand() {
    seed = (1103515245 * seed + rand()) % 2147483648;
    return seed;
}

int randint(const int min, const int max){
    return min + lcg_rand() % (max - min + 1);
}

double distance(double xa, double ya, double xb, double yb) {
    const double d = sqrt((xa - xb) * (xa - xb) + (ya - yb) * (ya - yb));
    return d;
}

SDL_Rect rect(const int x, const int y, const int w, const int h, const _Bool autoscale){
    SDL_Rect R = {x, y, w, h};
    if (autoscale) {
        R.w = scale_width(w);
        R.h = scale_height(h);
    }
    return R;
}

SDL_Color color(const int r, const int g, const int b, const int a){
    const SDL_Color C = {r, g, b, a};
    return C;
}

void centered_abs(SDL_Rect *rect){
    rect->x = WIDTH / 2 - rect->w / 2;
}

void centered_ord(SDL_Rect *rect){
    rect->y = HEIGHT / 2 - rect->h / 2;
}

void centered_coord(SDL_Rect *rect){
    rect->x = WIDTH / 2 - rect->w / 2;
    rect->y = HEIGHT / 2 - rect->h / 2;
}
