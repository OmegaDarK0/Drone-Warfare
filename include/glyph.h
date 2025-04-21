#ifndef GLYPH_H
#define GLYPH_H

#include <SDL2/SDL.h>

void CreateGlyphs();
void ClearGlyphs();
void RenderGlyphs();
SDL_Surface *GetGlyph(const char c);

#endif // GLYPH_H

