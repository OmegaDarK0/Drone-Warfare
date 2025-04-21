#include "graphics.h"
#include "engine.h"  // Pour QuitEngine() et le renderer global
#include <SDL2/SDL.h>
#include <math.h>

// Déclaration du renderer global défini dans engine.c
extern SDL_Renderer *renderer;

SDL_Rect FRectToRect(const SDL_FRect frect) {
    SDL_Rect rect;
    rect.x = (int)roundf(frect.x);
    rect.y = (int)roundf(frect.y);
    rect.w = (int)frect.w;
    rect.h = (int)frect.h;
    return rect;
}

void DrawPoint(const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderDrawPoint(renderer, x, y) < 0) {
        SDL_Log("SDL_RenderDrawPoint: %s", SDL_GetError());
        QuitEngine();
    }
}

void DrawLine(const int xa, const int ya, const int xb, const int yb, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderDrawLine(renderer, xa, ya, xb, yb) < 0) {
        SDL_Log("SDL_RenderDrawLine: %s", SDL_GetError());
        QuitEngine();
    }
}

void DrawRect(const SDL_Rect rect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderFillRect(renderer, &rect) < 0) {
        SDL_Log("SDL_RenderFillRect: %s", SDL_GetError());
        QuitEngine();
    }
}

void DrawRectF(const SDL_FRect frect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderFillRectF(renderer, &frect) < 0) {
        SDL_Log("SDL_RenderFillRectF: %s", SDL_GetError());
        QuitEngine();
    }
}

void DrawTexture(SDL_Texture *texture, const SDL_Rect rect) {
    if (texture) {
        if (SDL_RenderCopy(renderer, texture, NULL, &rect) < 0) {
            SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
            QuitEngine();
        }
    } else {
        // Si aucun texture n'est fournie, on remplit la zone en blanc par défaut.
        DrawRect(rect, 255, 255, 255, 255);
    }
}

void DrawBackground(SDL_Texture *texture) {
    if (texture) {
        if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0) {
            SDL_Log("SDL_RenderCopy (background): %s", SDL_GetError());
            QuitEngine();
        }
    }
}

void RefreshScreen(void) {
    SDL_RenderPresent(renderer);
}

void ClearScreen(void) {
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderClear(renderer) < 0) {
        SDL_Log("SDL_RenderClear: %s", SDL_GetError());
        QuitEngine();
    }
}

void SetPosition(SDL_Rect *rect, const int x, const int y) {
    rect->x = x;
    rect->y = y;
}

void SetPositionF(SDL_FRect *frect, const float x, const float y) {
    frect->x = x;
    frect->y = y;
}

_Bool Intersecting(const SDL_FRect frect1, const SDL_FRect frect2) {
    const SDL_Rect r1 = FRectToRect(frect1);
    const SDL_Rect r2 = FRectToRect(frect2);
    return SDL_HasIntersection(&r1, &r2);
}

void GetTextureSize(SDL_Texture *texture, int *width, int *height) {
    if (SDL_QueryTexture(texture, NULL, NULL, width, height) < 0) {
         SDL_Log("SDL_QueryTexture: %s", SDL_GetError());
         QuitEngine();
    }
}

Uint32 ColorToUint32(const SDL_Surface *surface, const SDL_Color color) {
    const Uint32 color32 = SDL_MapRGB(surface->format, color.r, color.g, color.b);
    if (color32 == 0) {
        SDL_Log("SDL_MapRGB: %s", SDL_GetError());
        QuitEngine();
    }
    return color32;
}

void ReplaceColor(const SDL_Surface *surface, const SDL_Color src_color, const SDL_Color dst_color) {
    const Uint32 src = ColorToUint32(surface, src_color);
    const Uint32 dst = ColorToUint32(surface, dst_color);
    Uint32 *pixels = surface->pixels;
    const int pixel_count = surface->pitch / 4 * surface->h;
    for (int i = 0; i < pixel_count; i++) {
        if (pixels[i] == src)
            pixels[i] = dst;
    }
}

