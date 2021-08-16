#include "core/input.h"

void Input::beginFrame() {
    pressedKeys.clear();
    releasedKeys.clear();
}

void Input::keyDown(const SDL_Event &event) {
    pressedKeys[event.key.keysym.scancode] = !heldKeys[event.key.keysym.scancode];
    heldKeys[event.key.keysym.scancode] = true;
}

void Input::keyUp(const SDL_Event &event) {
    releasedKeys[event.key.keysym.scancode] = true;
    heldKeys[event.key.keysym.scancode] = false;
}

bool Input::isKeyHeld(SDL_Scancode key) {
    return heldKeys[key];
}
bool Input::wasKeyPressed(SDL_Scancode key) {
    return pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
    return releasedKeys[key];
}
