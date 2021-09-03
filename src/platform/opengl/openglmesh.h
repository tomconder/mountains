#ifndef INCLUDE_OPENGLMESH_H
#define INCLUDE_OPENGLMESH_H

#include <memory>

#include "platform/opengl/openglbuffer.h"
#include "platform/opengl/openglelementbuffer.h"
#include "platform/opengl/opengltexture.h"
#include "platform/opengl/openglvertexarray.h"
#include "renderer/mesh.h"

class OpenGLMesh : public Mesh {
public:
    OpenGLMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<std::shared_ptr<OpenGLTexture>> &textures);
    void render() const;

    std::unique_ptr<OpenGLBuffer> vbo;
    std::unique_ptr<OpenGLElementBuffer> ebo;
    std::unique_ptr<OpenGLVertexArray> vao;
    std::vector<std::shared_ptr<OpenGLTexture>> textures;
};

#endif //INCLUDE_OPENGLMESH_H
