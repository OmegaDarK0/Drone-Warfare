#include "engine/window.h"
#include "engine/logger.h"
#include "engine/utils.h"

static const char *ns = "ENGINE";
static const char *module = "WINDOW";

static SDL_Window *window = NULL;

SDL_Window *Engine_GetWindow(void) { return window; }

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

void Engine_DestroyWindow(void) {
    if (window == NULL) {
        Engine_Log(LOG_WARN, ns, module, "Window already destroyed.");
    } else {
        SDL_DestroyWindow(window);
        window = NULL;
        Engine_Log(LOG_OK, ns, module, "Window destroyed successfully.");
    }
}
