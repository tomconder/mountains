#ifndef INCLUDE_OPENGLFONT_H
#define INCLUDE_OPENGLFONT_H

#include <map>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "platform/opengl/openglbuffer.h"
#include "platform/opengl/opengltexture.h"
#include "platform/opengl/openglvertexarray.h"

struct Character {
    unsigned int id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class OpenGLFont {
public:
    OpenGLFont();
    void load(const std::string &path, unsigned int fontSize);

    void renderText(const std::string &text, float x, float y, glm::vec3 color);

    std::unique_ptr<OpenGLBuffer> vbo;
    std::unique_ptr<OpenGLVertexArray> vao;

    std::map<unsigned char, Character> Characters;
};

#endif //INCLUDE_OPENGLFONT_H
