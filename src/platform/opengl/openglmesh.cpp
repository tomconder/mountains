#include "platform/opengl/openglmesh.h"

#ifdef EMSCRIPTEN
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

#include <cstddef>

#include "easylogging++.h"
#include "platform/opengl/openglresourcemanager.h"

OpenGLMesh::OpenGLMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
    this->vertices = vertices;
    this->indices = indices;

    vao = std::make_unique<OpenGLVertexArray>();
    vao->bind();

    vbo = std::make_unique<OpenGLBuffer>(reinterpret_cast<const float *>(vertices.data()),
                                         vertices.size() * sizeof(Vertex));
    vbo->bind();

    ebo = std::make_unique<OpenGLElementBuffer>(indices.data(), indices.size() * sizeof(unsigned int));
    ebo->bind();

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();

    GLuint program = shader->getId();

    auto position = static_cast<GLuint>(glGetAttribLocation(program, "position"));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    position = static_cast<GLuint>(glGetAttribLocation(program, "normal"));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    position = static_cast<GLuint>(glGetAttribLocation(program, "texCoords"));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    position = static_cast<GLuint>(glGetAttribLocation(program, "tangent"));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

    position = static_cast<GLuint>(glGetAttribLocation(program, "biTangent"));
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, biTangent));

    glBindVertexArray(0);
}

void OpenGLMesh::render() const {
    vao->bind();
    glActiveTexture(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
