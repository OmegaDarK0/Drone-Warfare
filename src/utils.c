#include "utils.h"
#include "constants.h"
#include "globals.h"
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

int scale_width(const int w) {
    return WIDTH * w / 1920;
}

int scale_height(const int h) {
    return HEIGHT * h / 1080;
}

double distance(const double xa, const double ya, const double xb, const double yb) {
    return sqrt((xa - xb) * (xa - xb) + (ya - yb) * (ya - yb));
}

SDL_Rect rect(const int x, const int y, const int w, const int h, const _Bool autoscale) {
    SDL_Rect R = {x, y, w, h};
    if (autoscale) {
        R.w = scale_width(w);
        R.h = scale_height(h);
    }
    return R;
}

SDL_Color color(const int r, const int g, const int b, const int a) {
    const SDL_Color C = {r, g, b, a};
    return C;
}

void centered_abs(SDL_Rect *rect) {
    rect->x = WIDTH / 2 - rect->w / 2;
}

void centered_ord(SDL_Rect *rect) {
    rect->y = HEIGHT / 2 - rect->h / 2;
}

void centered_coord(SDL_Rect *rect) {
    rect->x = WIDTH / 2 - rect->w / 2;
    rect->y = HEIGHT / 2 - rect->h / 2;
}

int file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

double set_velocity_x(const double var) {
    // Ajuste la vélocité selon une référence de 1920 pixels.
    return var / 1920.0 * WIDTH * 60;
}

double set_velocity_y(const double var) {
    // Ajuste la vélocité selon une référence de 1080 pixels.
    return var / 1080.0 * HEIGHT * 60;
}
