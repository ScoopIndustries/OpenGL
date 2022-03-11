#include "../Header/Buffer.hpp"


void Buffer::CreateBuffer(const GLfloat* iData, size_t iSize)
{
    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, iSize, iData, GL_STATIC_DRAW);
}

void Buffer::BindBuffer(int attribut)
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(attribut);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glVertexAttribPointer(
        attribut,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
}
