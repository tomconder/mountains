#ifndef INCLUDE_OPENGLMODEL_H
#define INCLUDE_OPENGLMODEL_H

#include <memory>
#include <string>

#include <assimp/scene.h>

#include "platform/opengl/openglmesh.h"
#include "renderer/mesh.h"

class OpenGLModel {
public:
    void load(const std::string &path);
    void render();

    std::vector<OpenGLMesh> meshes;

private:
    void processNode(const aiNode *node, const aiScene *scene);
    OpenGLMesh processMesh(const aiMesh *mesh, const aiScene *scene) const;
};

#endif //INCLUDE_OPENGLMODEL_H
