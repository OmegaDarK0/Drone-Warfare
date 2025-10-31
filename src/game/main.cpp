#include "engine/core.h"
#include "engine/logger.h"
#include "game/engine.hpp"

int GameMain() {
    try {
        EngineGuard engine;
        SDL_Delay(1500);
    } catch (const std::exception&) {
        return -1;
    }
    return 0;
}

