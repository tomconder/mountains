#ifndef INCLUDE_OPENGL_SHADER_H
#define INCLUDE_OPENGL_SHADER_H

#include <string>

#include <glad/gl.h>

#include "renderer/shader.h"

class OpenGLShader : public Shader {
public:
    OpenGLShader() = default;
    ~OpenGLShader() override;

    void compileAndLink(const std::string &vertexSource, const std::string &fragmentSource);
    GLuint getId() const { return program; };

    OpenGLShader *bind() override;

    void setInteger(const std::string &name, int value) override;
    void setMat4(const std::string &name, glm::mat4 value) override;
    void setFloat3(const std::string &name, glm::vec3 value) override;

private:
    static GLuint compileShader(GLenum type, const std::string &file);
    static GLuint linkProgram(GLuint vs, GLuint fs);

    GLuint program = 0;
};

#endif //INCLUDE_OPENGL_SHADER_H
