#pragma once

#include <glm/mat4x4.hpp>
#include "Mesh.h"

class RenderObject {
public:
   RenderObject(Mesh* mesh, glm::mat4 transform);
   Mesh* mesh;
   glm::mat4 modelTransform;
   std::string flag;

   void render();
};