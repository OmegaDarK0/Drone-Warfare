#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <SDL2/SDL.h>

#define RENDER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

#ifdef __cplusplus
extern "C" {
#endif

SDL_Renderer *Engine_GetRenderer(void);
int Engine_CreateRenderer(Uint32 flags);
void Engine_DestroyRenderer(void);

void Engine_Present(void);
int Engine_Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

int Engine_DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int Engine_DrawPointF(float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

#ifdef __cplusplus
}
#endif

#endif //ENGINE_RENDERER_H