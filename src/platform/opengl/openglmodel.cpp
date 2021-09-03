#include "platform/opengl/openglmodel.h"

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "platform/opengl/openglresourcemanager.h"
#include "easylogging++.h"

void OpenGLModel::load(const std::string &path) {
    assert(!path.empty());

    meshes.clear();

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_FindDegenerates |
                                                 aiProcess_FindInvalidData |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_GenSmoothNormals |
                                                 aiProcess_ImproveCacheLocality |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_LimitBoneWeights |
                                                 aiProcess_RemoveRedundantMaterials |
                                                 aiProcess_SplitLargeMeshes |
                                                 aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG(ERROR) << "Unable to load model: " << path;
        return;
    }

    processNode(scene->mRootNode, scene);

    auto textures = OpenGLResourceManager::getTextures();
    for (auto pair : textures) {
        auto texture = pair.second;
        LOG(INFO) << "texture[" << texture->getId() << ", " << texture->getType() << ", " << texture->getWidth() << "x" << texture->getWidth() << "]";
    }
}

void OpenGLModel::processNode(const aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

OpenGLMesh OpenGLModel::processMesh(const aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};

        glm::vec3 glmVec3;
        glmVec3.x = mesh->mVertices[i].x;
        glmVec3.y = mesh->mVertices[i].y;
        glmVec3.z = mesh->mVertices[i].z;
        vertex.position = glmVec3;

        if (mesh->HasNormals()) {
            glmVec3.x = mesh->mNormals[i].x;
            glmVec3.y = mesh->mNormals[i].y;
            glmVec3.z = mesh->mNormals[i].z;
            vertex.normal = glmVec3;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 glmVec2;

            // assume models have one set of texture coordinates
            glmVec2.x = mesh->mTextureCoords[0][i].x;
            glmVec2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = glmVec2;

            glmVec3.x = mesh->mTangents[i].x;
            glmVec3.y = mesh->mTangents[i].y;
            glmVec3.z = mesh->mTangents[i].z;
            vertex.tangent = glmVec3;

            glmVec3.x = mesh->mBitangents[i].x;
            glmVec3.y = mesh->mBitangents[i].y;
            glmVec3.z = mesh->mBitangents[i].z;
            vertex.biTangent = glmVec3;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    // 2. specular maps
    loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    // 3. normal maps
    loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    // 4. height maps
    loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

    return { vertices, indices };
}

void OpenGLModel::loadMaterialTextures(const aiMaterial *mat, aiTextureType textureType, const std::string &typeName) {
    for (unsigned int i = 0; i < mat->GetTextureCount(textureType); i++) {
        aiString str;
        mat->GetTexture(textureType, i, &str);

        std::string filename = std::string("assets/models/") + std::string(str.C_Str());
        auto texture = OpenGLResourceManager::loadTexture(filename, OpenGLResourceManager::basename(filename), typeName);
    }
}

void OpenGLModel::render() {
    for (auto it = std::begin(meshes); it != std::end(meshes); ++it) {
        it->render();
    }
}
