#ifndef INCLUDE_OPENGLMESH_H
#define INCLUDE_OPENGLMESH_H

#include <string>

#include <assimp/scene.h>

#include "renderer/mesh.h"

class OpenGLMesh {
public:
    OpenGLMesh();
    void load(const std::string &path);
    void render();

    std::vector<Mesh>    meshes;

private:
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};

#endif //INCLUDE_OPENGLMESH_H
