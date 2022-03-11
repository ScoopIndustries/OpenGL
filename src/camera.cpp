/*
Copyright 2022 Nicolas Colombe

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../Header/camera.hpp"
#include <glm/ext.hpp>

namespace GC_3D
{

    void Camera_Data::UpdateTransformFromMatrix()
    {
        mat4 camTransform = glm::inverse(viewMatrix);
        basis[0] = camTransform[0];
        basis[1] = camTransform[1];
        basis[2] = camTransform[2];
        position = camTransform[3];
    }

    void Camera_Data::UpdateMatrixFromTransform()
    {
        mat4 camTransform;
        camTransform[0] = glm::vec4(basis[0], 0);
        camTransform[1] = glm::vec4(basis[1], 0);
        camTransform[2] = glm::vec4(basis[2], 0);
        camTransform[3] = glm::vec4(position, 1);

        viewMatrix = glm::inverse(camTransform);
    }

    OrbitCamera::OrbitCamera()
        : OrbitCamera(vec3(0.0, 0.0, 1.0)
            , vec3(0.0, 0.0, 0.0)
            , vec3(0.0, 1.0, 0.0))
    {
    }

    OrbitCamera::OrbitCamera(vec3 const& iPos, vec3 const& iFocus, vec3 iUpDir)
    {
        m_Data.viewMatrix = glm::lookAt(iPos, iFocus, iUpDir);
        m_Focus = iFocus;
        m_Data.UpdateTransformFromMatrix();
    }

    void OrbitCamera::Reframe(BoundingSphere const& iSph)
    {
        m_Focus = iSph.m_Center;
        vec3 viewDir = m_Data.basis[2];

        m_Data.position = m_Focus - viewDir * iSph.m_Radius * 2.0f;

        m_Data.viewMatrix = glm::lookAt(m_Data.position, m_Focus, m_Data.basis[1]);
        m_Data.UpdateTransformFromMatrix();
    }

    void OrbitCamera::Update(vec3 const& iLocalMovement)
    {
        float dist = glm::length(m_Focus - m_Data.position);

        m_Data.position += iLocalMovement.x * m_Data.basis[0]
            + iLocalMovement.y * m_Data.basis[1];

        vec3 viewDir = m_Focus - m_Data.position;
        viewDir = glm::normalize(viewDir);
        m_Data.position = m_Focus - (dist + iLocalMovement.z) * viewDir;

        m_Data.viewMatrix = glm::lookAt(m_Data.position, m_Focus, m_Data.basis[1]);
        m_Data.UpdateTransformFromMatrix();
    }
}