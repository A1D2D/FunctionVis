#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ImageLoader {
   class Loader {
   public:
      Loader() {}
      ~Loader() {
         data.clear();
      }

      bool loadFromSrc(const std::string& src) {
         unsigned char* pixels = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(src.data()),
            static_cast<int>(src.size()),
            &size.x, &size.y, &nrChannels, 0
         );
         if (pixels) {
            data.clear();
            data.assign(pixels, pixels + size.x * size.y * nrChannels);
            stbi_image_free(pixels);
            return true;
         }
         return false;
      }

      std::vector<unsigned char> data;
      glm::ivec2 size;
      int nrChannels;
   };
}