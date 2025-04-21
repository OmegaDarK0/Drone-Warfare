#include "glyph.h"
#include "images.h"
#include "graphics.h"
#include <stdlib.h>

SDL_Surface *void_surface;
SDL_Surface **glyphs;

void CreateGlyphs() {
    int index = 0;
    void_surface = CreateImage(8, 8);
    glyphs = malloc(94 * sizeof(SDL_Surface*));
    SDL_Surface *ascii_surface = LoadImage("ascii.png");
    SDL_Surface *glyph;
    // Extraction des glyphes depuis ascii_surface (les positions sont fixes)
    for (int i = 0; i < 15; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8, 0);
        glyphs[index++] = glyph;
    }
    for (int i = 0; i < 10; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8, 9);
        glyphs[index++] = glyph;
    }
    for (int i = 0; i < 7; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8 + 80, 9);
        glyphs[index++] = glyph;
    }
    for (int i = 0; i < 26; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8, 18);
        glyphs[index++] = glyph;
    }
    for (int i = 0; i < 6; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8 + 120, 0);
        glyphs[index++] = glyph;
    }
    for (int i = 0; i < 26; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8, 27);
        glyphs[index++] = glyph;
    }
    for (int i = 0; i < 4; i++) {
        glyph = CreateImage(8, 8);
        CopyImage(ascii_surface, glyph, i * 8 + 136, 9);
        glyphs[index++] = glyph;
    }
    UnloadImage(ascii_surface);
}

void ClearGlyphs() {
    UnloadImage(void_surface);
    for (int i = 0; i < 94; i++)
        UnloadImage(glyphs[i]);
    free(glyphs);
}

void RenderGlyphs() {
    int index = 0;
    for (int i = 0; i < 15; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8, 0, 8, 8});
        UnloadTexture(t);
    }
    for (int i = 0; i < 10; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8, 9, 8, 8});
        UnloadTexture(t);
    }
    for (int i = 0; i < 7; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8 + 80, 9, 8, 8});
        UnloadTexture(t);
    }
    for (int i = 0; i < 26; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8, 18, 8, 8});
        UnloadTexture(t);
    }
    for (int i = 0; i < 6; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8 + 120, 0, 8, 8});
        UnloadTexture(t);
    }
    for (int i = 0; i < 26; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8, 27, 8, 8});
        UnloadTexture(t);
    }
    for (int i = 0; i < 4; i++) {
        SDL_Texture *t = LoadTextureFromImage(glyphs[index++]);
        DrawTexture(t, (SDL_Rect){i * 8 + 136, 9, 8, 8});
        UnloadTexture(t);
    }
}

SDL_Surface *GetGlyph(const char c) {
    const int index = (int)c - 33;
    if (index < 0 || index >= 94)
        return void_surface;
    return glyphs[index];
}

