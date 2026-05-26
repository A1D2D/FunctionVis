#include "RenderObject.h"

RenderObject::RenderObject(Mesh* mesh, glm::mat4 transform) : mesh(mesh), modelTransform(transform) {}

void RenderObject::render() {
   mesh->materialInstance->setUniform({"model", modelTransform});
   mesh->bindMaterial();
   mesh->renderMesh();
   mesh->unbindMaterial();
}

