#ifndef INCLUDE_SHADER_H
#define INCLUDE_SHADER_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>

class Shader {
public:
    virtual ~Shader() = default;

    virtual Shader *bind() = 0;

    virtual void setBoolean(const std::string &name, bool value) = 0;
    virtual void setFloat(const std::string &name, float value) = 0;
    virtual void setFloat3(const std::string &name, glm::vec3 value) = 0;
    virtual void setInteger(const std::string &name, int value) = 0;
    virtual void setMat4(const std::string &name, glm::mat4 value) = 0;
};

#endif //INCLUDE_SHADER_H
