#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define IMG_INIT_FLAGS (IMG_INIT_JPG | IMG_INIT_PNG)
#define MIX_INIT_FLAGS (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_WAVPACK)

int Engine_Init(void);
void Engine_Exit(void);

#endif //ENGINE_CORE_H