#include "engine/renderer.h"
#include "engine/window.h"
#include "engine/logger.h"
#include "engine/utils.h"

static const char *ns = "ENGINE";
static const char *module = "RENDER";

static SDL_Renderer *renderer = NULL;

SDL_Renderer *Engine_GetRenderer(void) { return renderer; }

int Engine_CreateRenderer(const Uint32 flags) {
    if (renderer != NULL) {
        Engine_Log(LOG_WARN, ns, module, "Renderer already created.");
        return WARNING;
    }
    SDL_Window *window = Engine_GetWindow();
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

void Engine_DestroyRenderer(void) {
    if (renderer == NULL) {
        Engine_Log(LOG_WARN, ns, module, "Renderer already destroyed.");
    } else {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
        Engine_Log(LOG_OK, ns, module, "Renderer destroyed successfully.");
    }
}

void Engine_Present(void) {
    SDL_RenderPresent(renderer);
}

int Engine_Clear(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_SetRenderDrawColor failed: %s", SDL_GetError());
        return  ERROR;
    }
    if (SDL_RenderClear(renderer) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_RenderClear failed: %s", SDL_GetError());
        return  ERROR;
    }
    return SUCCESS;
}

int Engine_DrawPoint(const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_SetRenderDrawColor failed: %s", SDL_GetError());
        return ERROR;
    }
    if (SDL_RenderDrawPoint(renderer, x, y) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_RenderDrawPoint failed: %s", SDL_GetError());
        return ERROR;
    }
    return SUCCESS;
}

int Engine_DrawPointF(const float x, const float y, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_SetRenderDrawColor failed: %s", SDL_GetError());
        return ERROR;
    }
    if (SDL_RenderDrawPointF(renderer, x, y) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_RenderDrawPointF failed: %s", SDL_GetError());
        return ERROR;
    }
    return SUCCESS;
}
