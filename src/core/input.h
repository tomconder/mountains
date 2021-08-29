#ifndef INCLUDE_INPUT_H
#define INCLUDE_INPUT_H

#include <SDL.h>
#include <map>
#include <glm/vec2.hpp>

typedef struct SDL_KeyboardEvent SDL_KeyboardEvent;
typedef struct SDL_MouseMotionEvent SDL_MouseMotionEvent;
typedef struct SDL_MouseWheelEvent SDL_MouseWheelEvent;

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

    glm::vec2 getDelta() { return delta; };

private:
    std::map<SDL_Scancode, bool> heldKeys;
    std::map<SDL_Scancode, bool> pressedKeys;
    std::map<SDL_Scancode, bool> releasedKeys;

    bool buttonPressed = false;
    glm::vec2 delta = {0.f, 0.f};
};

#endif //INCLUDE_INPUT_H
