#pragma once

#include <chrono>

#include <GL/glew.h>
#include <gl/GL.h>

#include <vector>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <sstream>


#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace GC_3D
{
  using namespace glm;

  template <typename T>
  using Vector = std::vector<T>;

  using Clock = std::chrono::high_resolution_clock;
  using Timestamp = Clock::time_point;
  using Duration = Clock::duration;

  inline float Seconds(Duration const& iDuration)
  {
    return std::chrono::duration_cast<std::chrono::duration<float>>(iDuration).count();
  }

  struct Geometry
  {
      //These arrays should all have the same size.
      //They can be empty as well
      Vector<vec3> m_Pos;
      Vector<vec3> m_Normals;
      Vector<vec2> m_TexCoord;

      Vector<uint32_t> m_Indices;

      void Bind() const;
      void Draw() const;
      Geometry MakeSphere(float radius);
  };

 

  struct BoundingSphere
  {
      vec3 m_Center;
      float m_Radius;
  };
    
}