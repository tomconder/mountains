#ifndef INCLUDE_OPENGLMESH_H
#define INCLUDE_OPENGLMESH_H

#include <memory>

#include "platform/opengl/openglbuffer.h"
#include "platform/opengl/openglelementbuffer.h"
#include "platform/opengl/openglvertexarray.h"
#include "renderer/mesh.h"

class OpenGLMesh : public Mesh {
public:
    OpenGLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void render();

    std::unique_ptr<OpenGLBuffer> vbo;
    std::unique_ptr<OpenGLElementBuffer> ebo;
    std::unique_ptr<OpenGLVertexArray> vao;
};

#endif //INCLUDE_OPENGLMESH_H
