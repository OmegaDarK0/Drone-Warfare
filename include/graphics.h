#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

// Conversion entre SDL_FRect et SDL_Rect
SDL_Rect FRectToRect(const SDL_FRect frect);

// Fonctions de dessin primitives
void DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawLine(int xa, int ya, int xb, int yb, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawRect(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawRectF(SDL_FRect frect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Fonctions de dessin de textures
void DrawTexture(SDL_Texture *texture, SDL_Rect rect);
void DrawBackground(SDL_Texture *texture);

// Fonctions pour rafraîchir et effacer l'écran
void RefreshScreen(void);
void ClearScreen(void);

// Fonctions utilitaires de positionnement
void SetPosition(SDL_Rect *rect, int x, int y);
void SetPositionF(SDL_FRect *frect, float x, float y);

// Détection d'intersection entre deux zones
_Bool Intersecting(SDL_FRect frect1, SDL_FRect frect2);

// Fonctions utilitaires liées aux textures
void GetTextureSize(SDL_Texture *texture, int *width, int *height);

// Fonctions pour la conversion et remplacement de couleurs dans une surface
Uint32 ColorToUint32(const SDL_Surface *surface, SDL_Color color);
void ReplaceColor(const SDL_Surface *surface, SDL_Color src_color, SDL_Color dst_color);

#endif // GRAPHICS_H
