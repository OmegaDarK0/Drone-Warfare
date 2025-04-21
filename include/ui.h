#ifndef UI_H
#define UI_H

#include "structures.h"
#include <SDL2/SDL.h>

// Création et gestion d'éléments "Solid" pour l'interface graphique
Solid *CreateSolid(SDL_Rect rect, SDL_Color color, const char *str);
Solid *CreateSolidText(SDL_Rect rect, SDL_Color color, const char *str, int size, SDL_Color text_color);
void DestroySolid(Solid *solid);

void DrawText(const Solid *text, _Bool centered);
void DrawButton(const Solid *button, _Bool centered, int pos_x, int pos_y, unsigned int mode);
void DisplayCursor(SDL_Texture *texture, SDL_Rect rect);

#endif // UI_H

