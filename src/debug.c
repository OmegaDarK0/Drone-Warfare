#include <SDL2/SDL_image.h>
#include "debug.h"
#include "globals.h"

SDL_Texture *icon = NULL;
SDL_Window *window_debug = NULL;
SDL_Renderer *renderer_debug = NULL;

void QuitDebug(){
    if (icon) SDL_DestroyTexture(icon);
    if (renderer_debug) SDL_DestroyRenderer(renderer_debug);
    if (window_debug) SDL_DestroyWindow(window_debug);
}

void InitDebug(){
    window_debug = SDL_CreateWindow("SDL2", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
    if (!window_debug){
        SDL_Log("SDL_CreateWindow: %s\n", SDL_GetError());
        QuitDebug();
    }
    renderer_debug = SDL_CreateRenderer(window_debug, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_debug){
        SDL_Log("SDL_CreateRenderer: %s\n", SDL_GetError());
        QuitDebug();
    }
    icon = IMG_LoadTexture(renderer_debug, "assets/images/drone0.png");
    if (!icon){
        SDL_Log("IMG_LoadTexture: %s\n", SDL_GetError());
        QuitDebug();
    }
}

void RenderDebug(const EntityList *entities){
    SDL_Rect rect = {16, 16, 32, 32};
    if (SDL_SetRenderDrawColor(renderer_debug, 0, 0, 0, 255) < 0){
        SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
        QuitDebug();
    }
    if (SDL_RenderClear(renderer_debug) < 0) {
        SDL_Log("SDL_RenderClear: %s\n", SDL_GetError());
        QuitDebug();
    }
    const EntityNode *current = entities->head;
    while (current){
        const EntityNode *next = current->next;
        if (SDL_RenderCopy(renderer_debug, icon, NULL, &rect) < 0){
            SDL_Log("SDL_RenderCopy: %s\n", SDL_GetError());
            QuitDebug();
        }
        rect.y += 48;
        current = next;
    }
    SDL_RenderPresent(renderer_debug);
        /*else{
            if (SDL_SetRenderDrawColor(renderer_debug, 255, 255, 255, 255) < 0){
                SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
                QuitDebug();
            }
            if (SDL_RenderFillRect(renderer_debug, &rect) < 0) {
                SDL_Log("SDL_RenderDrawRect: %s\n", SDL_GetError());
                QuitDebug();
            }
        }*/
}


