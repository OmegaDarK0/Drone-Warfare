#include "engine/renderer.h"
#include "engine/logger.h"
#include "engine/utils.h"

static char *ns = "ENGINE";
static char *module = "RENDER";

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_Window *Engine_GetWindow(void) { return window; }
SDL_Renderer *Engine_GetRenderer(void) { return renderer; }

int Engine_CreateWindow(const Uint32 flags) {
    if (window != NULL) {
        Engine_Log(LOG_WARN, ns, module, "Window already created.");
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
        Engine_Log(LOG_ERROR, ns, module, "SDL_CreateWindow failed: %s", SDL_GetError());
        return ERROR;
    }
    Engine_Log(LOG_OK, ns, module, "Window created successfully (%dx%d).", WIDTH, HEIGHT);
    return SUCCESS;
}

int Engine_CreateRenderer(const Uint32 flags) {
    if (renderer != NULL) {
        Engine_Log(LOG_WARN, ns, module, "Renderer already created.");
        return WARNING;
    }
    if (window == NULL) {
        Engine_Log(LOG_ERROR, ns, module, "Cannot create renderer: window is NULL.");
        return ERROR;
    }
    renderer = SDL_CreateRenderer(window, -1, flags);
    if (renderer == NULL) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_CreateRenderer failed: %s", SDL_GetError());
        return ERROR;
    }
    Engine_Log(LOG_OK, ns, module, "Renderer created successfully (flags=0x%X).", flags);
    return SUCCESS;
}

void Engine_DestroyWindow(void) {
    if (window == NULL) {
        Engine_Log(LOG_WARN, ns, module, "Window already destroyed.");
    } else {
        SDL_DestroyWindow(window);
        window = NULL;
        Engine_Log(LOG_OK, ns, module, "Window destroyed successfully.");
    }
}

void Engine_DestroyRenderer(void) {
    if (renderer == NULL) {
        Engine_Log(LOG_WARN, ns, module, "Renderer already destroyed.");
    } else {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
        Engine_Log(LOG_OK, ns, module, "Renderer destroyed successfully.");
    }
}
