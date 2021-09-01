#ifndef INCLUDE_OPENGLMODEL_H
#define INCLUDE_OPENGLMODEL_H

#include <memory>
#include <string>

#include <assimp/scene.h>

#include "platform/opengl/openglmesh.h"
#include "platform/opengl/opengltexture.h"
#include "renderer/mesh.h"

class OpenGLModel {
public:
    void load(const std::string &path);
    void render();

    std::vector<OpenGLMesh> meshes;
    std::vector<std::shared_ptr<OpenGLTexture>> textures;
    std::string name;

private:
    void processNode(const aiNode *node, const aiScene *scene);
    OpenGLMesh processMesh(const aiMesh *mesh, const aiScene *scene);
    static std::vector<std::shared_ptr<OpenGLTexture>> loadMaterialTextures(const aiMaterial *mat,
                                                                     aiTextureType type,
                                                                     const std::string &typeName) ;
};

#endif //INCLUDE_OPENGLMODEL_H
