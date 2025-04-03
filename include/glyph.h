#ifndef GLYPH_H
#define GLYPH_H
#include <SDL2/SDL_surface.h>

void CreateGlyphs();
void ClearGlyphs();
void RenderGlyphs();
SDL_Surface *GetGlyph(char c);

#endif //GLYPH_H
