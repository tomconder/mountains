#ifndef INCLUDE_TEXTURE_H
#define INCLUDE_TEXTURE_H

#include <string>

class Texture {
public:
    virtual ~Texture() = default;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual uint32_t getId() const = 0;

    virtual void bind() const = 0;
};

#endif //INCLUDE_TEXTURE_H
