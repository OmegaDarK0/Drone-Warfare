#include "engine/core.h"
#include "engine/window.h"
#include "engine/renderer.h"
#include "engine/logger.h"
#include "engine/utils.h"

static const char *ns = "ENGINE";
static const char *module = "CORE";

int Engine_Init(void) {
    _Bool warning = 0;
    int status = ERROR;
    Engine_Log(LOG_INFO, ns, module, "Initializing engine...");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Engine_Log(LOG_ERROR, ns, module, "SDL_Init failed: %s", SDL_GetError());
        goto end;
    }
    if ((IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS) != IMG_INIT_FLAGS) {
        Engine_Log(LOG_ERROR, ns, module, "IMG_Init failed: %s", IMG_GetError());
        goto end;
    }
    if ((Mix_Init(MIX_INIT_FLAGS) & MIX_INIT_FLAGS) != MIX_INIT_FLAGS) {
        Engine_Log(LOG_WARN, ns, module, "Mix_Init failed: %s (audio disabled)", Mix_GetError());
        warning = 1;
    } else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        Engine_Log(LOG_WARN, ns, module, "Mix_OpenAudio failed: %s (audio disabled)", Mix_GetError());
        warning = 1;
    } else if (Mix_AllocateChannels(32) < 0) {
        Engine_Log(LOG_WARN, ns, module, "Mix_AllocateChannels failed: %s (audio disabled)", Mix_GetError());
        warning = 1;
    }
    if (Engine_CreateWindow(WINDOW_FLAGS) == ERROR) goto end;
    if (Engine_CreateRenderer(RENDER_FLAGS) == ERROR) goto end;
    status = warning ? WARNING : SUCCESS;
    end:
        if (status == ERROR) {
            Engine_Log(LOG_ERROR, ns, module, "Engine initialization failed.");
            Engine_Exit();
        } else if (status == WARNING) {
            Engine_Log(LOG_WARN, ns, module, "Engine initializated with warnings.");
        } else {
            Engine_Log(LOG_OK, ns, module, "Engine initialized successfully.");
        }
    return status;
}

void Engine_Exit(void) {
    Engine_Log(LOG_INFO, ns, module, "Shutting down engine...");
    Engine_DestroyRenderer();
    Engine_DestroyWindow();
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    Engine_Log(LOG_OK, ns, module, "Engine shutdown complete.");
}
