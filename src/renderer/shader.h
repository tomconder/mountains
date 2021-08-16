#ifndef INCLUDE_SHADER_H
#define INCLUDE_SHADER_H

#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    virtual ~Shader() = default;

    virtual Shader *bind() = 0;

    virtual void setInteger(const std::string &name, int value) = 0;
    virtual void setMat4(const std::string &name, glm::mat4 value) = 0;
    virtual void setFloat3(const std::string &name, glm::vec3 value) = 0;
};

#endif //INCLUDE_SHADER_H
