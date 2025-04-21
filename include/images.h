#ifndef IMAGES_H
#define IMAGES_H

#include <SDL2/SDL.h>

// Création, chargement et conversion d'images
SDL_Surface *CreateImage(int width, int height);
SDL_Surface *LoadImage(const char *filename);
SDL_Texture *LoadTexture(const char *filename);
SDL_Texture *LoadTextureFromImage(SDL_Surface *surface);
SDL_Surface *LoadText(const char *text, int size, SDL_Color color);
SDL_Texture *LoadTextureFromText(const char *text, int size, SDL_Color color);

// Fonctions de déchargement
void UnloadImage(SDL_Surface *surface);
void UnloadTexture(SDL_Texture *texture);

// Manipulation d'images
void CopyImage(const SDL_Surface *src, const SDL_Surface *dst, int x_offset, int y_offset);
void ResizeImage(const SDL_Surface *src, const SDL_Surface *dst, int width, int height, int offset);
SDL_Surface* CreateSurfaceFromTexture(SDL_Renderer *renderer, SDL_Texture *texture);
void HighlightFilter(const SDL_Surface* surface, int alpha);

#endif // IMAGES_H

