#include "utils.h"
#include "constants.h"
#include "globals.h"
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

int ScaleWidth(const int w) {
    return WIDTH * w / 1920;
}

int ScaleHeight(const int h) {
    return HEIGHT * h / 1080;
}

double ScaleFPS(const double t) {
    return 60 * t / FPS;
}

int LCGRand(void) {
    seed = (1103515245 * seed + rand()) % 2147483648;
    return (int)seed;
}

int RandInt(const int min, const int max) {
    return min + LCGRand() % (max - min + 1);
}

double Distance(const double xa, const double ya, const double xb, const double yb) {
    return sqrt((xa - xb) * (xa - xb) + (ya - yb) * (ya - yb));
}

SDL_Rect Rect(const int x, const int y, const int w, const int h, const _Bool autoscale) {
    SDL_Rect R = {x, y, w, h};
    if (autoscale) {
        R.w = ScaleWidth(w);
        R.h = ScaleHeight(h);
    }
    return R;
}

SDL_Color Color(const int r, const int g, const int b, const int a) {
    const SDL_Color C = {r, g, b, a};
    return C;
}

void CenteredAbs(SDL_Rect *rect) {
    rect->x = WIDTH / 2 - rect->w / 2;
}

void CenteredOrd(SDL_Rect *rect) {
    rect->y = HEIGHT / 2 - rect->h / 2;
}

void CenteredCoord(SDL_Rect *rect) {
    rect->x = WIDTH / 2 - rect->w / 2;
    rect->y = HEIGHT / 2 - rect->h / 2;
}

int FileExists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

int TimeToFrames(const double seconds) {
    // Convertit le temps en secondes au nombre de frames, en se basant sur FPS.
    return (int)(seconds * FPS);
}

double SetVelocityX(const double var) {
    // Ajuste la vélocité selon une référence de 1920 pixels.
    return var / 1920.0 * WIDTH * DELTA_TIME * 60;
}

double SetVelocityY(const double var) {
    // Ajuste la vélocité selon une référence de 1080 pixels.
    return var / 1080.0 * HEIGHT * DELTA_TIME * 60;
}
