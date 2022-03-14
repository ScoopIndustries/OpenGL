#pragma once
#include "gc_3d_defs.hpp"

class Buffer
{
    GLuint m_buffer;
    

public:
    void CreateBuffer(const GLfloat* iData, size_t iSize);
    void BindBufferToAttrib(int attribut, int size, int stride, int arrayOffset);
};