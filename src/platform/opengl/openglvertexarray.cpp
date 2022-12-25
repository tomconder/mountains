#include "platform/opengl/openglvertexarray.h"

#ifdef EMSCRIPTEN
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

OpenGLVertexArray::OpenGLVertexArray() {
    glGenVertexArrays(1, &id);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &id);
}

void OpenGLVertexArray::bind() const {
    glBindVertexArray(id);
}
