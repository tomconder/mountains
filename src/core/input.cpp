#include "core/input.h"

#include "easylogging++.h"

void Input::beginFrame() {
    pressedKeys.clear();
    releasedKeys.clear();
}

void Input::keyDown(const SDL_KeyboardEvent &event) {
    pressedKeys[event.keysym.scancode] = !heldKeys[event.keysym.scancode];
    heldKeys[event.keysym.scancode] = true;
}

void Input::keyUp(const SDL_KeyboardEvent &event) {
    releasedKeys[event.keysym.scancode] = true;
    heldKeys[event.keysym.scancode] = false;
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

void Input::mouseButtonDown(const SDL_MouseButtonEvent &event) {
    if (event.button == SDL_BUTTON_LEFT) {
        buttonPressed = true;
    }
}

void Input::mouseButtonUp(const SDL_MouseButtonEvent &event) {
    if (event.button == SDL_BUTTON_LEFT) {
        buttonPressed = false;
    }
}

bool Input::isButtonPressed() const {
    return buttonPressed;
}

void Input::mouseMove(const SDL_MouseMotionEvent &event) {
    moveDelta = {event.xrel, event.yrel};
}

void Input::mouseScroll(const SDL_MouseWheelEvent &event) {
    scrollDelta = {static_cast<float>(event.timestamp), static_cast<float>(event.y)};
}

glm::vec2 Input::getScrollDelta() {
    if (lastScrollTimestamp < scrollDelta.x) {
        lastScrollTimestamp = scrollDelta.x;
        return scrollDelta;
    }
    return glm::vec2(0.f);
}

