#ifndef INCLUDE_GLOBALS_H
#define INCLUDE_GLOBALS_H

#define UNUSED(x) (void)(x)

namespace globals {
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum class Retcode {
    FAIL = 0,
    OK = 1
};
}

namespace direction {
enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};
}

namespace move {
enum class Move {
    LEFT,
    RIGHT,
    FORWARD,
    BACK,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    NONE
};
}

#endif //INCLUDE_GLOBALS_H
