#include "engine.h"
#include "constants.h"
#include "globals.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

int QuitEngine(){
    running = SDL_FALSE;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    //SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    SDL_Quit();
    return -1;
}

int InitEngine() {
    SDL_DisplayMode dm;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return QuitEngine();
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("IMG_Init: %s", IMG_GetError());
        return QuitEngine();
    }
    if ((Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC) & (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC))
        != (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC)) {
        SDL_Log("Mix_Init: %s", Mix_GetError());
        return QuitEngine();
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Mix_OpenAudio: %s", Mix_GetError());
        return QuitEngine();
    }
    if (Mix_AllocateChannels(32) < 0) {
        SDL_Log("Mix_AllocateChannels: %s", Mix_GetError());
        return QuitEngine();
    }
    if (SDL_GetCurrentDisplayMode(0, &dm) < 0) {
        SDL_Log("SDL_GetCurrentDisplayMode: %s", SDL_GetError());
        return QuitEngine();
    }
    WIDTH = dm.w;
    HEIGHT = dm.h;
    REFRESH_RATE = dm.refresh_rate;
    SCREEN = rect(0, 0, WIDTH, HEIGHT, 0);
    
    window = SDL_CreateWindow("Drone Warfare", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);
    if (!window) {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        return QuitEngine();
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        return QuitEngine();
    }
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
        SDL_Log("SDL_SetRenderDrawBlendMode: %s", SDL_GetError());
        return QuitEngine();
    }
    return 0;
}
