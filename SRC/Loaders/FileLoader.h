#pragma once
#include <fstream>
#include <sstream>

namespace FileLoader {
   inline std::string loadFile(const std::string& path) {
      std::ifstream file(path, std::ios::binary);
      if(!file.is_open()) return "";
      std::stringstream ss;
      ss << file.rdbuf();
      return ss.str();
   }
}