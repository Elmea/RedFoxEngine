#pragma once

#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
  struct Material
  {
      RedFoxMaths::Float3 diffuse;
      float Shininess;

      float Opaqueness;
      int diffuseMap;
      int normalMap;
      int _padding;
  };
}

