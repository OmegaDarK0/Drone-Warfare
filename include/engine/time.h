#ifndef ENGINE_TIME_H
#define ENGINE_TIME_H

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

inline Uint32 Engine_GetTicks() {
    return SDL_GetTicks();
}

inline void Engine_Delay(const Uint32 ms) {
    SDL_Delay(ms);
}

inline void Engine_Sleep(const float s) {
    SDL_Delay((Uint32)(s * 1000.0f));
}

#ifdef __cplusplus
}
#endif

#endif //ENGINE_TIME_H