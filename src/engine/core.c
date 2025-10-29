#include "engine/core.h"
#include "engine/renderer.h"
#include "engine/logger.h"
#include "engine/utils.h"

int Engine_Init(void) {
    int status = ERROR;
    Engine_Log(LOG_INFO, "[CORE] Initializing engine...");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Engine_Log(LOG_ERROR, "[CORE] SDL_Init failed: %s", SDL_GetError());
        goto exit;
    }
    if ((IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS) != IMG_INIT_FLAGS) {
        Engine_Log(LOG_ERROR, "[CORE] IMG_Init failed: %s", IMG_GetError());
        goto exit;
    }
    if ((Mix_Init(MIX_INIT_FLAGS) & MIX_INIT_FLAGS) != MIX_INIT_FLAGS) {
        Engine_Log(LOG_ERROR, "[CORE] Mix_Init failed: %s", Mix_GetError());
        goto exit;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        Engine_Log(LOG_ERROR, "[CORE] Mix_OpenAudio failed: %s", Mix_GetError());
        goto exit;
    }
    if (Mix_AllocateChannels(32) < 0) {
        Engine_Log(LOG_ERROR, "[CORE] Mix_AllocateChannels failed: %s", Mix_GetError());
        goto exit;
    }
    if (Engine_CreateWindow(WINDOW_FLAGS) == ERROR) goto exit;
    if (Engine_CreateRenderer(RENDER_FLAGS) == ERROR) goto exit;
    status = SUCCESS;
    exit:
        if (status == ERROR) {
            Engine_Log(LOG_ERROR, "[CORE] Engine initialization failed.");
            Engine_Exit();
        } else {
            Engine_Log(LOG_OK, "[CORE] Engine initialized successfully.");
        }
    return status;
}

void Engine_Exit(void) {
    Engine_Log(LOG_INFO, "[CORE] Shutting down engine...");
    Engine_DestroyRenderer();
    Engine_DestroyWindow();
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    Engine_Log(LOG_OK, "[CORE] Engine shutdown complete.");
}
