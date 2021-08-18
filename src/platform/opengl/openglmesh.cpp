#include "platform/opengl/openglmesh.h"

#ifdef EMSCRIPTEN
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

#include <cstddef>

OpenGLMesh::OpenGLMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) : Mesh(vertices,
                                                                                                             indices) {
    vao = std::make_unique<OpenGLVertexArray>();
    vao->bind();

    vbo = std::make_unique<OpenGLBuffer>(reinterpret_cast<const float *>(vertices.data()),
                                         vertices.size() * sizeof(Vertex));
    vbo->bind();

    ebo = std::make_unique<OpenGLElementBuffer>(indices.data(), indices.size() * sizeof(unsigned int));
    ebo->bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, biTangent));

    glBindVertexArray(0);
}

void OpenGLMesh::render() const {
    vao->bind();
    glActiveTexture(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
