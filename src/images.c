#include "images.h"
#include "constants.h"
#include "globals.h"
#include "glyph.h"
#include "graphics.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

SDL_Surface *CreateImage(const int width, const int height) {
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        SDL_Log("SDL_CreateRGBSurface: %s", SDL_GetError());
        // Possibilité d’appeler QuitEngine();
    }
    return surface;
}

SDL_Surface *LoadImage(const char *filename) {
    char path[256];
    sprintf(path, "assets/images/%s", filename);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("IMG_Load: %s", IMG_GetError());
        // Gestion d’erreur éventuellement
    }
    return surface;
}

SDL_Texture *LoadTexture(const char *filename) {
    char path[256];
    sprintf(path, "assets/images/%s", filename);
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        SDL_Log("IMG_LoadTexture: %s", IMG_GetError());
    }
    return texture;
}

SDL_Texture *LoadTextureFromImage(SDL_Surface *surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
    }
    return texture;
}

SDL_Surface *LoadText(const char *text, const int size, const SDL_Color color) {
    // Exemple simplifié : création d'une surface via des glyphes (fonction GetGlyph doit être définie dans le module Glyph)
    const int n = (int)strlen(text);
    const int scaled_size = scale_width(size);
    const int char_width = scaled_size;
    const int char_height = scaled_size;
    const int width = char_width * n + scaled_size * n / 8;
    const int height = char_height;
    SDL_Surface *dst = CreateImage(width, height);
    for (int i = 0; i < n; i++){
        const SDL_Surface *src = GetGlyph(text[i]);  // Appel à GetGlyph() issu du module Glyph
        ResizeImage(src, dst, char_width, char_height, char_width * i + scaled_size * i / 8);
        // Remplacer le blanc par la couleur souhaitée
        ReplaceColor(dst, WHITE, color);
    }
    return dst;
}

SDL_Texture *LoadTextureFromText(const char *text, const int size, const SDL_Color color) {
    SDL_Surface *surface = LoadText(text, size, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    UnloadImage(surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
    }
    return texture;
}

void UnloadImage(SDL_Surface *surface) {
    if (surface) SDL_FreeSurface(surface);
}

void UnloadTexture(SDL_Texture *texture) {
    if (texture) SDL_DestroyTexture(texture);
}

void CopyImage(const SDL_Surface *src, const SDL_Surface *dst, const int x_offset, const int y_offset) {
    const Uint32 *src_pixels = (Uint32 *)src->pixels;
    Uint32 *dst_pixels = (Uint32 *)dst->pixels;
    const int src_pitch = src->pitch / 4;
    const int dst_pitch = dst->pitch / 4;
    for (int y = 0; y < dst->h; y++) {
        for (int x = 0; x < dst->w; x++) {
            const int src_x = x + x_offset;
            const int src_y = y + y_offset;
            dst_pixels[y * dst_pitch + x] = src_pixels[src_y * src_pitch + src_x];
        }
    }
}

void ResizeImage(const SDL_Surface *src, const SDL_Surface *dst, const int width, const int height, const int offset) {
    const Uint32 *src_pixels = (Uint32 *)src->pixels;
    Uint32 *dst_pixels = (Uint32 *)dst->pixels;
    const int src_pitch = src->pitch / 4;
    const int dst_pitch = dst->pitch / 4;
    const int sw = src->w;
    const int sh = src->h;
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            const int sx = dx * sw / width;
            const int sy = dy * sh / height;
            const Uint32 pixel = src_pixels[sy * src_pitch + sx];
            dst_pixels[dy * dst_pitch + dx + offset] = pixel;
        }
    }
}

SDL_Surface* CreateSurfaceFromTexture(SDL_Renderer *renderer, SDL_Texture *texture) {
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        return NULL;
    }
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(renderer, NULL);
    return surface;
}

void HighlightFilter(const SDL_Surface* surface, const int alpha) {
    Uint32 *pixels = surface->pixels;
    const int pixel_count = surface->pitch / 4 * surface->h;
    for (int i = 0; i < pixel_count; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], surface->format, &r, &g, &b, &a);
        if (a) a = alpha;
        pixels[i] = SDL_MapRGBA(surface->format, r, g, b, a);
    }
}

