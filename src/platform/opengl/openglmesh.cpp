#include "openglmesh.h"

#include <vector>
#include <assert.h>

#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>

#include "easylogging++.h"

OpenGLMesh::OpenGLMesh() {
    // TODO
}

void OpenGLMesh::load(const std::string &path) {
    assert(!path.empty());

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
                                                 | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(ERROR) << "Unable to load model: " << path;
        return;
    }

    processNode(scene->mRootNode, scene);

}

void OpenGLMesh::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh OpenGLMesh::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

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

        if (mesh->mTextureCoords[0])
        {
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
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
//    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
//    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
//    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
//    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices);
}

void OpenGLMesh::render() {
    // TODO
}
