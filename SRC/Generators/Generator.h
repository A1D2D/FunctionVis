#pragma once
#include "glm/fwd.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Generator {
   template<typename T>
   struct Array2D {
      std::vector<T> height;
      glm::ivec2 size;

      void resize() {
         height.resize(size.x * size.y);
      }


      T& operator()(int x, int y) {
         return height[index(x, y)];
      }

      const T& operator()(int x, int y) const {
         return height[index(x, y)];
      }

      int index(int x, int y) const {
         return y * size.x + x;
      }
   };

   template<typename T>
   struct Array3D {
      std::vector<T> weight;
      glm::ivec3 size;

      T& operator()(int x, int y, int z) {
         return weight[index(x, y, z)];
      }

      const T& operator()(int x, int y, int z) const {
         return weight[index(x, y, z)];
      }

      void resize() {
         weight.resize(size.x * size.y * size.z);
      }

      int index(int x, int y, int z) const {
         return x + y * size.x + z * size.x * size.y;
      }
   };

   struct Vertex {
      glm::vec3 position;

      glm::vec3 normal;

      glm::vec2 textureCoordinate;
   };

   class SurfaceNets {
   public:
      struct Voxel {
         glm::vec3 pos;
         int index;
         int cornerMask;
      };
      std::vector<Vertex> Vertices;
      std::vector<unsigned int> Indices;

      void createMesh(const Array3D<float>& weights, glm::vec3 position, glm::vec3 scale, glm::vec2 uvScale, bool interpolation, bool smooth, float iso);
      
      ~SurfaceNets();
   };

   class HeightMap {
   public:
      std::vector<Vertex> Vertices;
      std::vector<unsigned int> Indices;

      void createMesh(const Array2D<float>& heights, glm::vec3 position, glm::vec3 scale, glm::vec2 uvScale, bool smooth = true);

      ~HeightMap();
   };
}