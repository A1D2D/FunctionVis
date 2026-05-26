#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../GraphicsApi/GraphicsApi.h"

class MaterialError : public std::exception {
   std::string message;
public:
   MaterialError(const std::string& msg) : message(msg) {}

   const char* what() const noexcept override {
      return message.c_str();
   }
};

class Material {
   public:
   Material(GraphicsApiI* api);
   void createMaterial(std::string vertSrc, std::string fragSrc);

   void bind();
   void setViewMatrix(glm::mat4 view);
   void setProjMatrix(glm::mat4 proj);
   void unbind();

   GraphicsApiI* api;
   std::vector<std::unique_ptr<ShaderI>> shaders;
   std::unique_ptr<ShaderProgramI> shaderProg;

   glm::mat4 viewMatrix;
   glm::mat4 projMatrix;
};

struct MInstanceUniform {
   enum UniformType {
      INTEGER,
      U_INTEGER,
      FLOATING,
      D_FLOATING,
      IVEC2,
      IVEC3,
      IVEC4,
      UVEC2,
      UVEC3,
      UVEC4,
      VEC2,
      VEC3,
      VEC4,
      DVEC2,
      DVEC3,
      DVEC4,
      MAT2,
      MAT3,
      MAT4,
      DMAT2,
      DMAT3,
      DMAT4
   };

   template<typename T>
   MInstanceUniform(const std::string& name, const T* values, int N) : varName(name), count(N), type(getType<T>()) {
      data.resize(sizeof(T) * N);
      memcpy(data.data(), values, sizeof(T) * N);
   }

   template<typename T>
   MInstanceUniform(const std::string& name, const std::vector<T>& values) : MInstanceUniform(name, values.data(), values.size()) {}
   
   template<typename T, int N>
   MInstanceUniform(const std::string& name, const T(&values)[N]) : MInstanceUniform(name, values, N) {}

   template<typename T>
   MInstanceUniform(const std::string& name, const T& value) : MInstanceUniform(name, &value, 1) {}
   
   template<typename T>
   static UniformType getType() {
      if constexpr(std::is_same_v<T, int>) return INTEGER;
      if constexpr(std::is_same_v<T, unsigned int>) return U_INTEGER;
      if constexpr(std::is_same_v<T, float>) return FLOATING;
      if constexpr(std::is_same_v<T, double>) return D_FLOATING;
      if constexpr(std::is_same_v<T, glm::ivec2>) return IVEC2;
      if constexpr(std::is_same_v<T, glm::ivec3>) return IVEC3;
      if constexpr(std::is_same_v<T, glm::ivec4>) return IVEC4;
      if constexpr(std::is_same_v<T, glm::uvec2>) return UVEC2;
      if constexpr(std::is_same_v<T, glm::uvec3>) return UVEC3;
      if constexpr(std::is_same_v<T, glm::uvec4>) return UVEC4;
      if constexpr(std::is_same_v<T, glm::vec2>) return VEC2;
      if constexpr(std::is_same_v<T, glm::vec3>) return VEC3;
      if constexpr(std::is_same_v<T, glm::vec4>) return VEC4;
      if constexpr(std::is_same_v<T, glm::dvec2>) return DVEC2;
      if constexpr(std::is_same_v<T, glm::dvec3>) return DVEC3;
      if constexpr(std::is_same_v<T, glm::dvec4>) return DVEC4;
      if constexpr(std::is_same_v<T, glm::mat2>) return MAT2;
      if constexpr(std::is_same_v<T, glm::mat3>) return MAT3;
      if constexpr(std::is_same_v<T, glm::mat4>) return MAT4;
      if constexpr(std::is_same_v<T, glm::dmat2>) return DMAT2;
      if constexpr(std::is_same_v<T, glm::dmat3>) return DMAT3;
      if constexpr(std::is_same_v<T, glm::dmat4>) return DMAT4;
   }

   std::string varName;
   UniformType type;
   unsigned int count;
   std::vector<unsigned char> data;
};

struct MInstanceSample {
   MInstanceSample(const std::string& name, ImageI* image);
   std::string name;
   ImageI* image;
}; 

class MaterialInstance {
public:
   MaterialInstance(Material* material);
   Material* material;
   std::vector<MInstanceUniform> uniforms;
   std::vector<MInstanceSample> samplers;
   void setUniform(MInstanceUniform uniform) {
      for (auto& u : uniforms) {
         if (u.varName == uniform.varName) {
            u = uniform;
            return;
         }
      }
      uniforms.push_back(uniform);
   }
   void setSampler(MInstanceSample sampler) {
      for (auto& s : samplers) {
         if (s.name == sampler.name) {
            s = sampler;
            return;
         }
      }
      samplers.push_back(sampler);
   }
   void bind();
   void unbind();
};
