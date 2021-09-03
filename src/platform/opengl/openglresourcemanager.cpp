#include "platform/opengl/openglresourcemanager.h"

#include <cassert>
#include <sstream>
#ifdef EMSCRIPTEN
#include <utility>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "easylogging++.h"

std::map<std::string, std::shared_ptr<OpenGLFont>, std::less<>> OpenGLResourceManager::fonts;
std::map<std::string, std::shared_ptr<OpenGLModel>, std::less<>> OpenGLResourceManager::meshes;
std::map<std::string, std::shared_ptr<OpenGLShader>, std::less<>> OpenGLResourceManager::shaders;
std::map<std::string, std::shared_ptr<OpenGLTexture>, std::less<>> OpenGLResourceManager::textures;

std::shared_ptr<OpenGLFont> OpenGLResourceManager::getFont(const std::string &name) {
    assert(!name.empty());
    return fonts.at(name);
}

std::shared_ptr<OpenGLFont> OpenGLResourceManager::loadFont(const std::string &path, const std::string &name) {
    assert(!path.empty());
    assert(!name.empty());

    if (fonts.find(name) != fonts.end()) {
        return fonts.at(name);
    }

    std::shared_ptr<OpenGLFont> font = loadFontFromFile(path);
    fonts.try_emplace(name, font);

    return font;
}

std::shared_ptr<OpenGLModel> OpenGLResourceManager::getMesh(const std::string &name) {
    assert(!name.empty());
    return meshes.at(name);
}

std::shared_ptr<OpenGLModel> OpenGLResourceManager::loadMesh(const std::string &path, const std::string &name) {
    assert(!path.empty());
    assert(!name.empty());

    if (meshes.find(name) != meshes.end()) {
        return meshes.at(name);
    }

    std::shared_ptr<OpenGLModel> mesh = loadMeshFromFile(path);
    meshes.try_emplace(name, mesh);

    return mesh;
}

std::shared_ptr<OpenGLShader> OpenGLResourceManager::getShader(const std::string &name) {
    assert(!name.empty());
    return shaders.at(name);
}

std::shared_ptr<OpenGLShader> OpenGLResourceManager::loadShader(const std::string &vertexShader,
                                                                const std::string &fragmentShader,
                                                                const std::string &name) {
    assert(!vertexShader.empty());
    assert(!fragmentShader.empty());
    assert(!name.empty());

    std::string vertexSource = loadSourceFromFile(vertexShader);
    std::string fragmentSource = loadSourceFromFile(fragmentShader);

    auto shader = std::make_shared<OpenGLShader>();
    shader->compileAndLink(vertexSource, fragmentSource);

    shaders.try_emplace(name, shader);
    return shader;
}

std::shared_ptr<OpenGLTexture> OpenGLResourceManager::getTexture(const std::string &name) {
    assert(!name.empty());
    return textures.at(name);
}

std::shared_ptr<OpenGLTexture> OpenGLResourceManager::loadTexture(const std::string &path, const std::string &name) {
    assert(!path.empty());
    assert(!name.empty());

    if (textures.find(name) != textures.end()) {
        return textures.at(name);
    }

    std::shared_ptr<OpenGLTexture> texture = loadTextureFromFile(path);
    textures.try_emplace(name, texture);

    return texture;
}

std::shared_ptr<OpenGLTexture> OpenGLResourceManager::loadTextureWithType(const std::string &path, const std::string &typeName) {
    assert(!path.empty());
    assert(!typeName.empty());

    if (textures.find(path) != textures.end()) {
        return textures.at(path);
    }

    std::shared_ptr<OpenGLTexture> texture = loadTextureFromFile(path);
    texture->setType(typeName);
    textures.try_emplace(path, texture);

    return texture;
}

std::shared_ptr<OpenGLFont> OpenGLResourceManager::loadFontFromFile(const std::string &path) {
    assert(!path.empty());

    LOG(INFO) << "Loading font: " << path;

    auto font = std::make_shared<OpenGLFont>();
    font->load(path, 24);

    return font;
}

std::shared_ptr<OpenGLModel> OpenGLResourceManager::loadMeshFromFile(const std::string &path) {
    assert(!path.empty());

    LOG(INFO) << "Loading mesh: " << path;

    auto mesh = std::make_shared<OpenGLModel>();
    mesh->load(path);

    return mesh;
}

std::string OpenGLResourceManager::loadSourceFromFile(const std::string &path) {
    assert(!path.empty());

    LOG(INFO) << "Loading shader: " << path;

    std::string code;
    if (std::ifstream stream(path, std::ios::in); stream.is_open()) {
        std::stringstream sstr;
        sstr << stream.rdbuf();
        code = sstr.str();
        stream.close();
    } else {
        LOG(ERROR) << "Unable to open " << path << ". Are you in the right directory?";
    }

    return code;
}

std::shared_ptr<OpenGLTexture> OpenGLResourceManager::loadTextureFromFile(const std::string &path) {
    assert(!path.empty());

    auto name = path;
    std::replace(name.begin(), name.end(), '\\', '/');

    LOG(INFO) << "Loading texture: " << name;

    SDL_Surface *surface = IMG_Load(name.c_str());
    if (surface == nullptr) {
        LOG(ERROR) << "Unable to load texture: " << path;
        return nullptr;
    }

    auto texture = std::make_shared<OpenGLTexture>();
    texture->generate(surface->w,
                      surface->h,
                      surface->format->BytesPerPixel,
                      static_cast<unsigned char *>(surface->pixels));

    SDL_FreeSurface(surface);

    return texture;
}
