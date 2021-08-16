#ifndef INCLUDE_OPENGLVERTEXARRAY_H
#define INCLUDE_OPENGLVERTEXARRAY_H

#include <SDL.h>

#include "renderer/vertexarray.h"

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void bind() const override;

private:
    uint32_t id = 0;
};

#endif //INCLUDE_OPENGLVERTEXARRAY_H
