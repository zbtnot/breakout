#include "Game.h"

Game *g = nullptr;

void loop() {
    if (g != nullptr) {
        g->frame();
    }
}

void init(const char *title) {
    if (g == nullptr) {
        g = new Game(title);
    }
}

void deinit() {
    if (g != nullptr) {
        delete g;
    }
}

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
extern "C" {
EMSCRIPTEN_KEEPALIVE void loadCat() {
    if (g == nullptr) {
        return;
    }
    emscripten_pause_main_loop();
    g->loadCat();
    emscripten_resume_main_loop();
}
}
#endif

int main(int argc, char **argv) {
    const char *title = argv[0];
    const int TARGET_FPS = 60;

#ifdef __EMSCRIPTEN__
    title = emscripten_get_window_title();
    init(title);
    emscripten_set_main_loop(loop, TARGET_FPS, 1);
#else
    init(title);
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        loop();
    }
    deinit();
#endif
}
