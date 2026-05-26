#include "Material.h"

#include <iostream>

Material::Material(GraphicsApiI* api) : api(api), shaderProg(api->createShaderProgram()) {}

void Material::createMaterial(std::string vertSrc, std::string fragSrc) {
   std::unique_ptr<ShaderI> vertShader(api->createShader(GraphicsApiI::ShaderType::VERTEX));
   std::unique_ptr<ShaderI> fragShader(api->createShader(GraphicsApiI::ShaderType::FRAGMENT));

   if(!vertShader->compile(vertSrc)) {
      std::cout << "vertex compilation failed:\n" << vertShader->getError();
   }

   if(!fragShader->compile(fragSrc)) {
      std::cout << "fragment compilation failed:\n" << fragShader->getError();
   }

   if(!shaderProg->attachShader(vertShader.get())) {
      std::cout << "failed to attach vert shader\n";
   }
   if(!shaderProg->attachShader(fragShader.get())) {
      std::cout << "failed to attach frag shader\n";
   }
   if(!shaderProg->linkProgram()) {
      std::cout << "" << shaderProg->getError();
   }
}

void Material::bind() {
   if(!shaderProg) return;
   shaderProg->useProgram();
   shaderProg->setUniform("projection", projMatrix);
   shaderProg->setUniform("view", viewMatrix);
}

void Material::setViewMatrix(glm::mat4 view) {
   viewMatrix = view;
}

void Material::setProjMatrix(glm::mat4 proj) {
   projMatrix = proj;
}

void Material::unbind() {
}

MaterialInstance::MaterialInstance(Material* material) : material(material) {}

void MaterialInstance::bind() {
   if(!material) return;
   material->bind();
   ShaderProgramI* shader = material->shaderProg.get();
   for (auto& unfirom : uniforms) {
      
      switch (unfirom.type) {
         case MInstanceUniform::INTEGER:
            shader->setUniform(unfirom.varName, reinterpret_cast<const int*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::U_INTEGER:
            shader->setUniform(unfirom.varName, reinterpret_cast<const unsigned int*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::FLOATING:
            shader->setUniform(unfirom.varName, reinterpret_cast<const float*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::D_FLOATING:
            shader->setUniform(unfirom.varName, reinterpret_cast<const double*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::IVEC2:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::ivec2*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::IVEC3:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::ivec3*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::IVEC4:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::ivec4*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::UVEC2:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::uvec2*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::UVEC3:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::uvec3*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::UVEC4:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::uvec4*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::VEC2:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::vec2*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::VEC3:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::vec3*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::VEC4:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::vec4*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::DVEC2:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::dvec2*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::DVEC3:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::dvec3*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::DVEC4:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::dvec4*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::MAT2:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::mat2*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::MAT3:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::mat3*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::MAT4:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::mat4*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::DMAT2:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::dmat2*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::DMAT3:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::dmat3*>(unfirom.data.data()), unfirom.count);
            break;
         case MInstanceUniform::DMAT4:
            shader->setUniform(unfirom.varName, reinterpret_cast<const glm::dmat4*>(unfirom.data.data()), unfirom.count);
            break;
      }
   }

   for (auto& sampler : samplers) {
      sampler.image->bind();
      material->shaderProg->setUniform(sampler.name, *sampler.image);
   }
}

void MaterialInstance::unbind() {
   if(!material) return;
   for (auto& sampler : samplers) {
      sampler.image->unbind();
   }
   material->unbind();
}

MInstanceSample::MInstanceSample(const std::string& name, ImageI* image) : name(name), image(image) {}
