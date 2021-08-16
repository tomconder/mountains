#ifndef INCLUDE_GRAPHICS_CONTEXT_H
#define INCLUDE_GRAPHICS_CONTEXT_H

#include "glm/vec4.hpp"

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;

    virtual void flip() = 0;
};

#endif //INCLUDE_GRAPHICS_CONTEXT_H
