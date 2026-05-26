#include "Mesh.h"
#include "glad/glad.h"
#include <memory>

Mesh::Mesh(GraphicsApiI* api) : 
   api(api), 
   vertices(api->createBuffer(GraphicsApiI::BufferType::VERTEX)), 
   indicies(api->createBuffer(GraphicsApiI::BufferType::INDEX)), 
   vao(api->createVertexLayout()) {
   vao->addBuffer(vertices.get());
   vao->addBuffer(indicies.get());

   struct VertexConfig {
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TextureCoordinate;
   };

   vao->addAttribLayout({0, 0, 3, GraphicsApiI::ApiDataType::FLOAT, sizeof(VertexConfig), offsetof(VertexConfig, Position)});
   vao->addAttribLayout({0, 1, 3, GraphicsApiI::ApiDataType::FLOAT, sizeof(VertexConfig), offsetof(VertexConfig, Normal)});
   vao->addAttribLayout({0, 2, 2, GraphicsApiI::ApiDataType::FLOAT, sizeof(VertexConfig), offsetof(VertexConfig, TextureCoordinate)});
}

void Mesh::bindMaterial() {
   if(!materialInstance) return;
   materialInstance->bind();
}

void Mesh::unbindMaterial() {
   if(!materialInstance) return;
   materialInstance->unbind();
}

void Mesh::renderMesh() {
   vao->bind();
   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
   vao->unbind();
}
