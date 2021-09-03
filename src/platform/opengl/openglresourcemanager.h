#ifndef INCLUDE_OPENGLRESOURCEMANAGER_H
#define INCLUDE_OPENGLRESOURCEMANAGER_H

#include <map>
#include <string>
#include <memory>

#include "platform/opengl/openglfont.h"
#include "platform/opengl/openglmodel.h"
#include "platform/opengl/openglshader.h"
#include "platform/opengl/opengltexture.h"

class OpenGLResourceManager {
public:
    static std::shared_ptr<OpenGLFont> getFont(const std::string &name);
    static std::shared_ptr<OpenGLFont> loadFont(const std::string &path, const std::string &name);

    static std::shared_ptr<OpenGLModel> getMesh(const std::string &name);
    static std::shared_ptr<OpenGLModel> loadMesh(const std::string &path, const std::string &name);

    static std::shared_ptr<OpenGLShader> loadShader(const std::string &vertexShader,
                                                    const std::string &fragmentShader,
                                                    const std::string &name);
    static std::shared_ptr<OpenGLShader> getShader(const std::string &name);

    static std::shared_ptr<OpenGLTexture> getTexture(const std::string &name);
    static std::shared_ptr<OpenGLTexture> loadTexture(const std::string &path, const std::string &name);
    static std::shared_ptr<OpenGLTexture> loadTextureWithType(const std::string &path, const std::string &typeName);

private:
    OpenGLResourceManager() = default;

    static std::shared_ptr<OpenGLFont> loadFontFromFile(const std::string &path);
    static std::shared_ptr<OpenGLModel> loadMeshFromFile(const std::string &path);
    static std::string loadSourceFromFile(const std::string &path);
    static std::shared_ptr<OpenGLTexture> loadTextureFromFile(const std::string &path);

    static std::map<std::string, std::shared_ptr<OpenGLFont>, std::less<>> fonts;
    static std::map<std::string, std::shared_ptr<OpenGLModel>, std::less<>> meshes;
    static std::map<std::string, std::shared_ptr<OpenGLShader>, std::less<>> shaders;
    static std::map<std::string, std::shared_ptr<OpenGLTexture>, std::less<>> textures;
};

#endif //INCLUDE_OPENGLRESOURCEMANAGER_H
