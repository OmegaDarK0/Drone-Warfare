#include "game/engine.hpp"
#include "engine/utils.h"

int main(const int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    Engine engine;
    Renderer& render {engine.renderer};
    Color colors[]{RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA};
    for (const auto & color : colors) {
        render.clear(color);
        render.present();
        engine.sleep(0.5);
    }
    return 0;
}
