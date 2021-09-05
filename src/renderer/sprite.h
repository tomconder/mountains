#ifndef INCLUDE_SPRITE_H
#define INCLUDE_SPRITE_H

#include <glm/vec2.hpp>
#include <string>

class Sprite {
public:
    virtual ~Sprite() = default;

    virtual void render(const std::string &name, glm::vec2 position, glm::vec2 size) const = 0;
};

#endif //INCLUDE_SPRITE_H
