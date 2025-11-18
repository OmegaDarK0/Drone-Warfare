#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <SDL2/SDL.h>

#define TITLE "TEMPLATE"

#define WIDTH 800
#define HEIGHT 600

#define WINDOW_FLAGS SDL_WINDOW_SHOWN

SDL_Window *Engine_GetWindow(void);
int Engine_CreateWindow(Uint32 flags);
void Engine_DestroyWindow(void);

#endif //ENGINE_WINDOW_H