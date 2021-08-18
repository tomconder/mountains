#ifndef INCLUDE_MESH_H
#define INCLUDE_MESH_H

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 biTangent;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif //INCLUDE_MESH_H
