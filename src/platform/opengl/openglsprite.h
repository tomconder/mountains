#ifndef INCLUDE_OPENGLSPRITE_H
#define INCLUDE_OPENGLSPRITE_H

#include <memory>

#include "glm/vec2.hpp"

#include "platform/opengl/openglbuffer.h"
#include "platform/opengl/openglvertexarray.h"

class OpenGLSprite {
public:
    OpenGLSprite();

    void render(const std::string &name, glm::vec2 position, glm::vec2 size) const;

    std::unique_ptr<OpenGLBuffer> vbo;
    std::unique_ptr<OpenGLVertexArray> vao;
};

#endif //INCLUDE_OPENGLSPRITE_H
