#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <iostream>
#include <mutex>

#include "engine/core.h"
#include "engine/logger.h"
#include "engine/time.h"
#include "engine/renderer.h"

inline std::string ns{"GAME"};
inline std::string module{"ENGINE"};

class Window {};

class Renderer {
public:
    void present() { Engine_Present(); }
    int clear() { return Engine_Clear(0, 0, 0, 255); }
    int clear(
        const Uint8 r,
        const Uint8 g,
        const Uint8 b,
        const Uint8 a = 255
        ) { return Engine_Clear(r, g, b, a); }
    int clear(const Uint32 color) {
        const Uint8 r = color >> 24 & 0xFF;
        const Uint8 g = color >> 16 & 0xFF;
        const Uint8 b = color >> 8 & 0xFF;
        const Uint8 a = color & 0xFF;
        return Engine_Clear(r, g, b, a);
    }
    int draw_point(const int x, const int y) { return Engine_DrawPoint(x, y, 255, 0, 0, 255); }
    int draw_point(const int x, const int y,
        const Uint8 r,
        const Uint8 g,
        const Uint8 b,
        const Uint8 a = 255
        ) { return Engine_DrawPoint(x, y, r, g, b, a); }
    int draw_point(const int x, const int y, const Uint32 color) {
        const Uint8 r = color >> 24 & 0xFF;
        const Uint8 g = color >> 16 & 0xFF;
        const Uint8 b = color >> 8 & 0xFF;
        const Uint8 a = color & 0xFF;
        return Engine_DrawPoint(x, y, r, g, b, a);
    }
    int draw_point(const float x, const float y) { return Engine_DrawPointF(x, y, 255, 0, 0, 255); }
    int draw_point(const float x, const float y,
        const Uint8 r,
        const Uint8 g,
        const Uint8 b,
        const Uint8 a = 255
        ) { return Engine_DrawPointF(x, y, r, g, b, a); }
    int draw_point(const float x, const float y, const Uint32 color) {
        const Uint8 r = color >> 24 & 0xFF;
        const Uint8 g = color >> 16 & 0xFF;
        const Uint8 b = color >> 8 & 0xFF;
        const Uint8 a = color & 0xFF;
        return Engine_DrawPointF(x, y, r, g, b, a);
    }
};

class Engine {
    static std::mutex instance_mutex;
    static int instance_count;
    static int global_id;
    int id{0};
public:
    Engine() {
        std::lock_guard lock(instance_mutex);
        id = global_id++;
        if (instance_count == 0) {
            if (init() < 0) {
                std::exit(EXIT_FAILURE);
            }
        }
        ++instance_count;
        log(LOG_INFO, "Created engine instance #%d (module='%s', active=%d)",
            id, module.c_str(), instance_count);
    }
    ~Engine() {
        std::lock_guard lock(instance_mutex);
        log(LOG_INFO, "Destroyed engine instance #%d (module='%s', remaining=%d)",
            id, module.c_str(), instance_count - 1);
        if (--instance_count == 0) {
            exit();
        }
    }
    int init() { return Engine_Init(); }
    void exit() { Engine_Exit(); }
    void delay(const Uint32 ms) { Engine_Delay(ms); }
    void sleep(const float s) { Engine_Sleep(s); }
    void log(const Log_Level level, const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        Engine_VLog(level, ns.c_str(), module.c_str(), fmt, args);
        va_end(args);
    }
    Window window;
    Renderer renderer;
};

std::mutex Engine::instance_mutex;
int Engine::instance_count = 0;
int Engine::global_id = 0;

#endif //GAME_ENGINE_HPP
