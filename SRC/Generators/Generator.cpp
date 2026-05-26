#include "Generator.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <iostream>
#include <vector>

namespace Generator {
   static const std::vector<glm::ivec3> normalLookup = {
      {-1,0,1},{0,1,1},{1,0,1},{0,-1,1}
   };

   static const std::vector<glm::ivec2> triangulationLookup = {
      {0,1},{1,0},{0,0},
      {0,1},{1,1},{1,0},

      {0,0},{1,1},{1,0},
      {0,0},{0,1},{1,1}
   };

   static const std::vector<glm::ivec3> cornerLookup = {
      {1, 1, 1}, {0, 1, 1}, {1, 0, 1}, {0, 0, 1}, 
      {1, 1, 0}, {0, 1, 0}, {1, 0, 0}, {0, 0, 0}
   };

   static const std::vector<std::vector<glm::ivec3>> faceLookup = {
      {{0, 0, 0}, {0, 1, 1}, {0, 1, 0}, {0, 0, 1}},
      {{0, 0, 0}, {1, 0, 1}, {0, 0, 1}, {1, 0, 0}},
      {{0, 0, 0}, {1, 1, 0}, {1, 0, 0}, {0, 1, 0}}
   };

   static const std::vector<glm::ivec2> edgeLookup = {
      {0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3}, 
      {2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}
   };

   static const std::vector<int> intersectionLookup = {
      0, 7, 25, 30, 98, 101, 123, 124, 168, 175, 177, 182, 202, 205, 211, 212, 
      772, 771, 797, 794, 870, 865, 895, 888, 940, 939, 949, 946, 974, 969, 983, 976, 
      1296, 1303, 1289, 1294, 1394, 1397, 1387, 1388, 1464, 1471, 1441, 1446, 1498, 1501, 1475, 1476, 
      1556, 1555, 1549, 1546, 1654, 1649, 1647, 1640, 1724, 1723, 1701, 1698, 1758, 1753, 1735, 1728, 
      2624, 2631, 2649, 2654, 2594, 2597, 2619, 2620, 2792, 2799, 2801, 2806, 2698, 2701, 2707, 2708, 
      2372, 2371, 2397, 2394, 2342, 2337, 2367, 2360, 2540, 2539, 2549, 2546, 2446, 2441, 2455, 2448, 
      3920, 3927, 3913, 3918, 3890, 3893, 3883, 3884, 4088, 4095, 4065, 4070, 3994, 3997, 3971, 3972, 
      3156, 3155, 3149, 3146, 3126, 3121, 3119, 3112, 3324, 3323, 3301, 3298, 3230, 3225, 3207, 3200, 
      3200, 3207, 3225, 3230, 3298, 3301, 3323, 3324, 3112, 3119, 3121, 3126, 3146, 3149, 3155, 3156, 
      3972, 3971, 3997, 3994, 4070, 4065, 4095, 4088, 3884, 3883, 3893, 3890, 3918, 3913, 3927, 3920, 
      2448, 2455, 2441, 2446, 2546, 2549, 2539, 2540, 2360, 2367, 2337, 2342, 2394, 2397, 2371, 2372, 
      2708, 2707, 2701, 2698, 2806, 2801, 2799, 2792, 2620, 2619, 2597, 2594, 2654, 2649, 2631, 2624, 
      1728, 1735, 1753, 1758, 1698, 1701, 1723, 1724, 1640, 1647, 1649, 1654, 1546, 1549, 1555, 1556, 
      1476, 1475, 1501, 1498, 1446, 1441, 1471, 1464, 1388, 1387, 1397, 1394, 1294, 1289, 1303, 1296, 
      976, 983, 969, 974, 946, 949, 939, 940, 888, 895, 865, 870, 794, 797, 771, 772, 
      212, 211, 205, 202, 182, 177, 175, 168, 124, 123, 101, 98, 30, 25, 7, 0,
   };

   glm::vec3 calculateTriangleNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
      glm::vec3 edge1 = b - a;
      glm::vec3 edge2 = c - a;

      return glm::normalize(glm::cross(edge1, edge2));
   }

   void HeightMap::createMesh(const Array2D<float>& heightMap, glm::vec3 position, glm::vec3 scale, glm::vec2 uvScale, bool smooth) {
      Indices.clear();
      Vertices.clear();

      if(smooth) {
         Vertices.reserve(heightMap.size.x * heightMap.size.y);
         Indices.reserve((heightMap.size.x-1)*(heightMap.size.y-1)*6);

         for (int i = 0; i < heightMap.size.x; i++) {
            for (int j = 0; j < heightMap.size.y; j++) {
               Vertex vertex;
               glm::vec3 basePos = glm::vec3(i, j, heightMap(i, j));
               vertex.position = basePos * scale + position;
               vertex.textureCoordinate = glm::vec2(basePos.x, basePos.y) * uvScale;

               vertex.normal = glm::vec3(0,0,1);
               for (int k = 0; k < normalLookup.size(); k++) {
                  glm::ivec3 shift = normalLookup[k];
                  glm::ivec2 shifted(i + shift.x, j + shift.y);
                  if(shifted.x < 0 || shifted.y < 0 || shifted.x >= heightMap.size.x || shifted.y >= heightMap.size.y) continue;
                  float dif = heightMap(i, j)-heightMap(shifted.x, shifted.y);
                  dif*=scale.z;
                  vertex.normal.x += dif * (float)shift.z * shift.x * scale.x;
                  vertex.normal.y += dif * (float)shift.z * shift.y * scale.y;
               }
               
               vertex.normal = glm::normalize(vertex.normal);
               Vertices.push_back(vertex);
            }
         }

         for(int i = 0; i < heightMap.size.x-1; i++) {
            for (int j = 0; j < heightMap.size.y-1; j++) {
               for (int vID = 0; vID < 6; vID++) {
                  glm::ivec2 c(i, j);
                  c += triangulationLookup[vID + ((i+j) % 2) * 6];
                  Indices.push_back(heightMap.index(c.x, c.y));
               }
            }
         }

      } else {
         Vertices.reserve((heightMap.size.x-1)*(heightMap.size.y-1)*6);
         Indices.reserve((heightMap.size.x-1)*(heightMap.size.y-1)*6);

         int index = 0;
         for(int i = 0; i < heightMap.size.x-1; i++) {
            for (int j = 0; j < heightMap.size.y-1; j++) {
               for (int t = 0; t < 2; t++) {
                  Vertex vertex[3];
                  for (int vID = 0; vID < 3; vID++) {
                     glm::ivec2 c(i, j);
                     c += triangulationLookup[t*3 + vID + ((i+j) % 2) * 6];
                     glm::vec3 basePos = glm::vec3(c.x, c.y, heightMap(c.x, c.y));
                     vertex[vID].position = basePos * scale + position;
                     vertex[vID].textureCoordinate = glm::vec2(basePos.x, basePos.y) * uvScale;
                  }
                  glm::vec3 normal = calculateTriangleNormal(vertex[2].position, vertex[1].position, vertex[0].position);
                  vertex[0].normal = normal;
                  vertex[1].normal = normal;
                  vertex[2].normal = normal;
                  Vertices.push_back(vertex[2]);
                  Vertices.push_back(vertex[1]);
                  Vertices.push_back(vertex[0]);

                  Indices.push_back(index++);
                  Indices.push_back(index++);
                  Indices.push_back(index++);
               }
            }
         }
      }
   }

   HeightMap::~HeightMap() {
      Indices.clear();
      Vertices.clear();
   }

   void SurfaceNets::createMesh(const Array3D<float>& weights, glm::vec3 position, glm::vec3 scale, glm::vec2 uvScale, bool interpolation, bool smooth, float iso) {
      Indices.clear();
      Vertices.clear();

      Array3D<Voxel> voxels;
      voxels.size = weights.size;
      voxels.resize();
      int index = 0;
      for (int x = 0; x < weights.size.x-1; x++) {
         for (int y = 0; y < weights.size.y-1; y++) {
            for (int z = 0; z < weights.size.z-1; z++) {
               //----
               int cornerMask = 0;
               float values[8];

               for (int c = 0; c < 8; ++c) {
                  values[c] = weights(x + cornerLookup[c].x, y + cornerLookup[c].y, z + cornerLookup[c].z);
                  if (values[c] > iso) {
                     cornerMask |= (1 << c);
                  }
               }

               voxels(x,y,z).cornerMask = cornerMask;
               if(cornerMask == 0 || cornerMask == 0xff) continue;

               glm::vec3 vertPos(0.5f);

               if(interpolation) {
                  int edgeCrossings = 0;
                  int edgeMask = intersectionLookup[cornerMask];
                  for (int e = 0; e < 12; e++) {
                     if (!((edgeMask & (1 << e)) > 0)) continue;
                     edgeCrossings++;
                     int c0 = edgeLookup[e].x;
                     int c1 = edgeLookup[e].y;
                     float t = (iso - values[c0]) / (values[c1] - values[c0]);

                     vertPos += glm::mix(glm::vec3(cornerLookup[c0]), glm::vec3(cornerLookup[c1]), t)*0.9999f;
                  }

                  vertPos /= (float)edgeCrossings;
               }

               vertPos += glm::vec3(x, y, z);
               voxels(x,y,z).pos = vertPos;
               voxels(x,y,z).index = index++;
               if(smooth) {
                  Vertex vert;
                  vert.position = vertPos * scale + position;
                  vert.textureCoordinate = glm::vec2(vertPos.x, vertPos.y) * uvScale;
                  vert.normal = glm::vec3(0,1,0);

                  Vertices.push_back(vert);
               }
               //----
            }
         }
      }

      //Triangulation
      index = 0;
      for (int x = 0; x < weights.size.x-2; x++) {
         for (int y = 0; y < weights.size.y-2; y++) {
            for (int z = 0; z < weights.size.z-2; z++) {
               Voxel& v = voxels(x, y, z);
               if(v.cornerMask == 0 || v.cornerMask == 0xff) continue;

               for (int f = 0; f < 3; f++) {
                  if(!(intersectionLookup[v.cornerMask] & (1 << f))) continue;

                  auto face = faceLookup[f];
                  
                  Voxel corners[4];
                  for (int c = 0; c < 4; c++) {
                     corners[c] = voxels(x+face[c].x, y+face[c].y, z+face[c].z);
                  }

                  Voxel vec[3][2];
                  if(0 != (v.cornerMask & 1)) {
                     vec[0][0] = corners[0];
                     vec[1][0] = corners[1];
                     vec[2][0] = corners[2];
                     vec[0][1] = corners[0];
                     vec[1][1] = corners[3];
                     vec[2][1] = corners[1];
                  } else {
                     vec[0][0] = corners[0];
                     vec[1][0] = corners[1];
                     vec[2][0] = corners[3];
                     vec[0][1] = corners[0];
                     vec[1][1] = corners[2];
                     vec[2][1] = corners[1];
                  }

                  for (int t = 0; t < 2; t++) {
                     if(smooth) {
                        int id[3] = {
                           vec[0][t].index,
                           vec[1][t].index,
                           vec[2][t].index
                        };

                        Indices.push_back(id[0]);
                        Indices.push_back(id[1]);
                        Indices.push_back(id[2]);
                        
                        glm::vec3 vec0 = Vertices[id[0]].position;
                        glm::vec3 vec1 = Vertices[id[1]].position;
                        glm::vec3 vec2 = Vertices[id[2]].position;

                        glm::vec3 normal = calculateTriangleNormal(vec0, vec1, vec2);
                        // if(std::isnan(normal.x) || std::isnan(normal.y) || std::isnan(normal.z)) {
                        //    std::cerr << "invalid normal\n";
                        // }

                        Vertices[id[0]].normal += normal;
                        Vertices[id[1]].normal += normal;
                        Vertices[id[2]].normal += normal;

                     } else {
                        Vertex v[3];
                        for (int tc = 0; tc < 3; tc++) {
                           Indices.push_back(index++);
                           
                           v[tc].position = vec[tc][t].pos * scale + position;
                           v[tc].textureCoordinate = glm::vec2(vec[tc][t].pos.x, vec[tc][t].pos.y) * uvScale;
                        }
                        glm::vec3 normal = calculateTriangleNormal(v[0].position, v[1].position, v[2].position);
                        v[0].normal = normal;
                        v[1].normal = normal;
                        v[2].normal = normal;
                        Vertices.push_back(v[0]);
                        Vertices.push_back(v[1]);
                        Vertices.push_back(v[2]);
                     }
                  }
               }
            }
         }
      }

      if(smooth) {
         for(auto& vert : Vertices) {
            if(vert.normal.length() < 0.001f) std::cerr << "normal error/n";
            vert.normal = glm::normalize(vert.normal);
         }
      }
   }

   SurfaceNets::~SurfaceNets() {
      Indices.clear();
      Vertices.clear();
   }
}