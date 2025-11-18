#include "game/engine.hpp"

int main(const int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    Engine engine;
    Renderer& render {engine.renderer};
    render.clear();
    render.draw_point(400, 400);
    render.present();
    engine.sleep(1);
    return 0;
}
