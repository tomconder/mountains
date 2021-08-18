#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &inVertices, const std::vector<unsigned int> &inIndices)
    : vertices(inVertices), indices(inIndices) {
// empty
}
