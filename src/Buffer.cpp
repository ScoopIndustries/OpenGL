#include "../Header/Buffer.hpp"


void Buffer::CreateBuffer(const GLfloat* iData, size_t iSize)
{
    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, iSize, iData, GL_STATIC_DRAW);
}

void Buffer::BindBufferToAttrib(int attribut, int size, int stride, int arrayOffset)
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(attribut);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glVertexAttribPointer(
        attribut,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        size,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        stride,                  // stride
        (void*)arrayOffset            // array buffer offset
    );
}
