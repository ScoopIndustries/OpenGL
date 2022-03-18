#pragma once
#include "../Header/gc_3d_defs.hpp"
#include "../Header/Buffer.hpp"
#include "../Header/shader.hpp"

namespace GC_3D
{
    void Geometry::Bind() const
    {
        Buffer m_Buffer;
        m_Buffer.CreateBuffer(CubeVertices, sizeof(CubeVertices));
        m_Buffer.BindBufferToAttrib(0, 3, 5 * sizeof(float), 0);
        m_Buffer.BindBufferToAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float)));
    }

    mat4 Geometry::CalculateMatrice(vec3 Position, bool rotateShape, float angle, float i, Shader shader, float scale) const
    {
        Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Position) * glm::scale(glm::vec3(scale, scale, scale));
        if(rotateShape)
        {
            float Rangle = angle * i;
            model = glm::rotate(model, glm::radians(Rangle), glm::vec3(1.0f, 0.3f, 0.5f));
        }

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        return model;
    }

    void Geometry::Draw()
    {

    }

    Geometry Geometry::MakeSphere(float iRadius)
    {
        unsigned int subDivHemi = 30;
        unsigned int subDivHeight = subDivHemi / 2;

        float stepPhi = 2 * pi<float>() / subDivHemi;
        float stepTheta = pi<float>() / subDivHeight;

        uint32_t const numVtx = (subDivHemi * (subDivHeight - 1) + 2);

        Geometry geom;
        geom.m_Pos.resize(numVtx);
        geom.m_Normals.resize(numVtx);
        //geom.m_TexCoord.resize(24);

        vec3* posPtr = geom.m_Pos.data();
        vec3* normalPtr = geom.m_Normals.data();
        *posPtr = vec3(0.0, iRadius, 0.0);
        *normalPtr = vec3(0.0, 1.0, 0.0);
        ++posPtr;
        ++normalPtr;
        for (unsigned int j = 0; j < subDivHemi - 1; ++j)
        {
            geom.m_Indices.push_back(0);
            geom.m_Indices.push_back(1 + j);
            geom.m_Indices.push_back(2 + j);
        }
        geom.m_Indices.push_back(0);
        geom.m_Indices.push_back(1 + subDivHemi - 1);
        geom.m_Indices.push_back(1);

        for (unsigned int i = 1; i < subDivHeight; ++i)
        {
            float theta = stepTheta * (float(subDivHeight) / 2.0 - i);
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            for (unsigned int j = 0; j < subDivHemi; ++j)
            {
                if (i != subDivHeight - 1)
                {
                    if (j != subDivHemi - 1)
                    {
                        geom.m_Indices.push_back(1 + (i - 1) * subDivHemi + j);
                        geom.m_Indices.push_back(1 + i * subDivHemi + j);
                        geom.m_Indices.push_back(2 + i * subDivHemi + j);
                        geom.m_Indices.push_back(1 + (i - 1) * subDivHemi + j);
                        geom.m_Indices.push_back(2 + i * subDivHemi + j);
                        geom.m_Indices.push_back(2 + (i - 1) * subDivHemi + j);
                    }
                    else
                    {
                        geom.m_Indices.push_back(1 + (i - 1) * subDivHemi + j);
                        geom.m_Indices.push_back(1 + i * subDivHemi + j);
                        geom.m_Indices.push_back(1 + i * subDivHemi);
                        geom.m_Indices.push_back(1 + (i - 1) * subDivHemi + j);
                        geom.m_Indices.push_back(1 + i * subDivHemi);
                        geom.m_Indices.push_back(1 + (i - 1) * subDivHemi);
                    }
                }

                float phi = j * stepPhi;
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);
                *normalPtr = vec3(cosPhi, 0, sinPhi) * cosTheta + vec3(0, sinTheta, 0);
                *normalPtr = normalize(*normalPtr);
                *posPtr = iRadius * *normalPtr;
                ++posPtr;
                ++normalPtr;
            }
        }

        *posPtr = vec3(0.0, -iRadius, 0.0);
        *normalPtr = vec3(0.0, -1.0, 0.0);
        ++posPtr;
        ++normalPtr;
        for (unsigned int j = 0; j < subDivHemi - 1; ++j)
        {
            geom.m_Indices.push_back(1 + (subDivHeight - 2) * subDivHemi + j);
            geom.m_Indices.push_back(2 + (subDivHeight - 2) * subDivHemi + j);
            geom.m_Indices.push_back(numVtx - 1);
        }
        geom.m_Indices.push_back(1 + (subDivHeight - 2) * subDivHemi + subDivHemi - 1);
        geom.m_Indices.push_back(1 + (subDivHeight - 2) * subDivHemi);
        geom.m_Indices.push_back(numVtx - 1);

        return geom;
    }
}