#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define TITLE "DRONE WARFARE"

#define WIDTH 1920
#define HEIGHT 1080

#define WINDOW_FLAGS SDL_WINDOW_FULLSCREEN_DESKTOP
#define RENDER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

SDL_Window *Engine_GetWindow(void);
SDL_Renderer *Engine_GetRenderer(void);
int Engine_CreateWindow(Uint32 flags);
int Engine_CreateRenderer(Uint32 flags);
void Engine_DestroyWindow(void);
void Engine_DestroyRenderer(void);

#endif //ENGINE_RENDERER_H