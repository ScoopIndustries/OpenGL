/*
Copyright 2022 Nicolas Colombe

Permission is hereby granted, free of charge, to any person obtaining a copy of this softwareand associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "gc_3d_defs.hpp"

namespace GC_3D
{
    struct Camera_Data
    {
        void UpdateTransformFromMatrix();
        void UpdateMatrixFromTransform();
        // World space to View space transform
        mat4 viewMatrix;

        // Camera position and orientation
        // AKA View space to World space transform.
        vec3 basis[3];
        vec3 position;

    };

    class OrbitCamera
    {
    public:
        OrbitCamera();
        OrbitCamera(vec3 const& iPos, vec3 const& iFocus, vec3 iUpDir);

        // Moves camera alongside its local axis.
        void Update(vec3 const& iLocalMovement);

        // Set the camera's focus on the given object
        void Reframe(BoundingSphere const& iSph);

        Camera_Data const& GetCamData() const { return m_Data; }

    protected:
        Camera_Data m_Data;
        vec3 m_Focus;
    };
}

