#ifndef GLYPH_H
#define GLYPH_H

#include <SDL2/SDL.h>

void CreateGlyphs();
void ClearGlyphs();
void RenderGlyphs();
SDL_Surface *GetGlyph(char c);
void CreateGlyphsWS();
void ClearGlyphsWS();
void RenderGlyphsWS();
SDL_Surface *GetGlyphWS(char c);

#endif // GLYPH_H

