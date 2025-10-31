#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <iostream>

#ifdef _WIN32
#define GameMain WinMain
#else
#define GameMain main
#endif

static std::string ns = "GAME";
static std::string module = "ENGINE";

struct EngineGuard {
    EngineGuard() {
        try {
            Engine_Log(LOG_INFO, ns.c_str(), module.c_str(), "Starting game engine...");
            if (Engine_Init() < 0)
                throw std::runtime_error("Engine initialization failed.");
        } catch (const std::exception& e) {
            Engine_Log(LOG_ERROR, ns.c_str(), module.c_str(), e.what());
            throw;
        }
    }

    ~EngineGuard() {
        Engine_Exit();
        Engine_Log(LOG_OK, ns.c_str(), module.c_str(), "Game engine terminated cleanly.");
    }
};

#endif //GAME_ENGINE_HPP
