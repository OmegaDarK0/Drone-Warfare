#include "engine/renderer.h"
#include "engine/logger.h"
#include "engine/utils.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_Window *Engine_GetWindow(void) { return window; }
SDL_Renderer *Engine_GetRenderer(void) { return renderer; }

int Engine_CreateWindow(const Uint32 flags) {
    if (window != NULL) {
        Engine_Log(LOG_WARN, "[RENDER] Window already created.");
        return WARNING;
    }
    window = SDL_CreateWindow(
        TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        flags
        );
    if (window == NULL) {
        Engine_Log(LOG_ERROR, "[RENDER] SDL_CreateWindow failed: %s", SDL_GetError());
        return ERROR;
    }
    Engine_Log(LOG_OK, "[RENDER] Window created successfully (%dx%d).", WIDTH, HEIGHT);
    return SUCCESS;
}

int Engine_CreateRenderer(const Uint32 flags) {
    if (renderer != NULL) {
        Engine_Log(LOG_WARN, "[RENDER] Renderer already created.");
        return WARNING;
    }
    if (window == NULL) {
        Engine_Log(LOG_ERROR, "[RENDER] Cannot create renderer: window is NULL.");
        return ERROR;
    }
    renderer = SDL_CreateRenderer(window, -1, flags);
    if (renderer == NULL) {
        Engine_Log(LOG_ERROR, "[RENDER] SDL_CreateRenderer failed: %s", SDL_GetError());
        return ERROR;
    }
    Engine_Log(LOG_OK, "[RENDER] Renderer created successfully (flags=0x%X).", flags);
    return SUCCESS;
}

void Engine_DestroyWindow(void) {
    if (window == NULL) {
        Engine_Log(LOG_WARN, "[RENDER] Window already destroyed.");
    } else {
        SDL_DestroyWindow(window);
        window = NULL;
        Engine_Log(LOG_OK, "[RENDER] Window destroyed successfully.");
    }
}

void Engine_DestroyRenderer(void) {
    if (renderer == NULL) {
        Engine_Log(LOG_WARN, "[RENDER] Renderer already destroyed.");
    } else {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
        Engine_Log(LOG_OK, "[RENDER] Renderer destroyed successfully.");
    }
}
