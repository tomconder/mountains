#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif
#include <SDL.h>

#include "easylogging++.h"
#include "mountains.h"

INITIALIZE_EASYLOGGINGPP

std::unique_ptr<Mountains> mountains;

// loop iteration is broken out like this for emscripten
bool iterateLoop() {
    return mountains->iterateLoop();
}

extern "C" int main(int argc, char *args[]) {
    START_EASYLOGGINGPP(argc, args);

    mountains = std::make_unique<Mountains>();

    if (mountains->construct(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT) == globals::Retcode::FAIL) {
        return 1;
    }

    if (mountains->start() == globals::Retcode::FAIL) {
        return 1;
    }

#ifdef EMSCRIPTEN
    emscripten_set_main_loop((em_callback_func)iterateLoop, 60, true);
#else
    bool quit = false;
    while (!quit) {
        quit = iterateLoop();
    }
#endif

    return 0;
}
