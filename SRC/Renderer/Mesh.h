#pragma once
#include <memory>

#include "../GraphicsApi/GraphicsApi.h"
#include "Material.h"

class Mesh {
public:
   Mesh(GraphicsApiI* api);

   template<typename T>
   void createMesh(const std::vector<T>& vertex, const std::vector<unsigned int>& index, MaterialInstance* mat) {
      vertices->setData(vertex);
      indicies->setData(index);
      materialInstance = mat;

      indexCount = index.size();
   }
   void bindMaterial();
   void unbindMaterial();
   void renderMesh();

   std::unique_ptr<BufferI> vertices;
   std::unique_ptr<BufferI> indicies;
   std::unique_ptr<VertexLayoutI> vao;
   int indexCount = 0;
   MaterialInstance* materialInstance;
   GraphicsApiI* api;
};