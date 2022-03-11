#include "../Header/gc_3d_defs.hpp"

namespace GC_3D
{
    void Geometry::Bind() const
    {
        // Set each attribute pointer, if we have data for it.
        if (!m_Pos.empty())
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(vec3), m_Pos.data());
        }
        else
        {
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        if (!m_Normals.empty())
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, sizeof(vec3), m_Normals.data());
        }
        else
        {
            glDisableClientState(GL_NORMAL_ARRAY);
        }
        if (!m_TexCoord.empty())
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, sizeof(vec2), m_TexCoord.data());
        }
        else
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    void Geometry::Draw() const
    {
        if (!m_Indices.empty())
        {
            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, m_Indices.data());
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, m_Pos.size());
        }
    }
}