#include "platform/opengl/openglshader.h"

#include <cassert>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include "easylogging++.h"

void OpenGLShader::compileAndLink(const std::string &vertexSource, const std::string &fragmentSource) {
    assert(!vertexSource.empty());
    assert(!fragmentSource.empty());

    LOG(INFO) << "Building program";

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    program = linkProgram(vs, fs);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(program);
}

OpenGLShader *OpenGLShader::bind() {
    glUseProgram(program);
    return this;
}

GLuint OpenGLShader::compileShader(const GLenum type, const std::string &source) {
    GLuint id = glCreateShader(type);
    assert(id != 0);

    LOG(INFO) << "Compiling shader";

    char const *shader = source.c_str();
    glShaderSource(id, 1, &shader, nullptr);
    glCompileShader(id);

    GLint result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            std::string message;
            message.resize(length);
            glGetShaderInfoLog(id, length, &length, message.data());
            LOG(ERROR) << message;
            glDeleteShader(id);
            return 0;
        }
    }

    return id;
}

GLuint OpenGLShader::linkProgram(GLuint vs, GLuint fs) {
    GLuint id = glCreateProgram();

    glAttachShader(id, vs);
    glAttachShader(id, fs);

    LOG(INFO) << "Linking program";

    glLinkProgram(id);

    GLint result = GL_FALSE;

    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            std::string message;
            message.resize(length);
            glGetShaderInfoLog(id, length, &length, message.data());
            LOG(ERROR) << message;
        }
    }

    glValidateProgram(id);

    return id;
}

void OpenGLShader::setBoolean(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), static_cast<int>(value));
}

void OpenGLShader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void OpenGLShader::setFloat3(const std::string &name, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
}

void OpenGLShader::setInteger(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void OpenGLShader::setMat4(const std::string &name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
