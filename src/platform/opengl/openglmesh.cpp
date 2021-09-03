#include "platform/opengl/openglmesh.h"

#ifdef EMSCRIPTEN
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

#include <cstddef>

#include "easylogging++.h"
#include "platform/opengl/openglresourcemanager.h"

OpenGLMesh::OpenGLMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<std::shared_ptr<OpenGLTexture>> &textures) : textures(textures) {
    this->indices = indices;
    this->vertices = vertices;

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

    position = static_cast<GLuint>(glGetAttribLocation(program, "texCoord"));
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

    std::shared_ptr<OpenGLShader> shader = OpenGLResourceManager::getShader("shader");
    shader->bind();

    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    unsigned int normalNumber = 1;
    unsigned int heightNumber = 1;
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i]->getType();
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNumber++);
        } else if (name == "texture_specular") {
            number = std::to_string(specularNumber++);
        } else if(name == "texture_normal") {
            number = std::to_string(normalNumber++);
        } else if(name == "texture_height") {
            number = std::to_string(heightNumber++);
        }

        shader->setInteger(name + number, i);
        textures[i]->bind();
    }

    if (textures.empty()) {
        shader->setBoolean("hasNoTexture", true);
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
