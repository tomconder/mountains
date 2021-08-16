#ifndef INCLUDE_VERTEXARRAY_H
#define INCLUDE_VERTEXARRAY_H

class VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;
};

#endif //INCLUDE_VERTEXARRAY_H
