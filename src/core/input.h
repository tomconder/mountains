#ifndef INCLUDE_INPUT_H
#define INCLUDE_INPUT_H

#include <SDL.h>
#include <map>
#include <glm/vec2.hpp>

class Input {
public:
    void beginFrame();
    void keyDown(const SDL_KeyboardEvent &event);
    void keyUp(const SDL_KeyboardEvent &event);

    bool isKeyHeld(SDL_Scancode key);
    bool wasKeyPressed(SDL_Scancode key);
    bool wasKeyReleased(SDL_Scancode key);

    void mouseButtonDown(const SDL_MouseButtonEvent &event);
    void mouseButtonUp(const SDL_MouseButtonEvent &event);

    bool isButtonPressed() const;
    void mouseMove(const SDL_MouseMotionEvent &event);
    void mouseScroll(const SDL_MouseWheelEvent &event);

    glm::vec2 getMoveDelta() const { return moveDelta; }
    glm::vec2 getScrollDelta();

private:
    std::map<SDL_Scancode, bool> heldKeys;
    std::map<SDL_Scancode, bool> pressedKeys;
    std::map<SDL_Scancode, bool> releasedKeys;

    bool buttonPressed = false;
    glm::vec2 moveDelta = {0.f, 0.f};
    glm::vec2 scrollDelta = {0.f, 0.f};
    float lastScrollTimestamp = 0.f;
};

#endif //INCLUDE_INPUT_H
