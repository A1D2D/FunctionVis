#include "LuaEngine.h"

#include <FastNoiseLite.h>

#include "../Generators/Generator.h"
#include "../Renderer/Mesh.h"
#include "Engine.h"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>

LuaEngine::LuaEngine(GraphicsApiI* api, Engine* engine) : api(api), engine(engine) {
   lua.open_libraries(sol::lib::base);
   using Array2Df = Generator::Array2D<float>;
   using Array3Df = Generator::Array3D<float>;

   noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
   noise.SetFrequency(0.1f);

   lua.new_enum(
      "NoiseType",

      "PERLIN", FastNoiseLite::NoiseType_Perlin,
      "CELLULAR", FastNoiseLite::NoiseType_Cellular,
      "SIMPLEX", FastNoiseLite::NoiseType_OpenSimplex2
   );

   lua.new_usertype<Array2Df>(
      "Array2D",

      sol::constructors<Array2Df()>(),

      "size", &Array2Df::size,

      "resize", &Array2Df::resize,

      "index", &Array2Df::index,

      "get", static_cast<float&(Array2Df::*)(int,int)>(&Array2Df::operator()),

      "set", [](Array2Df& a, int x, int y, float v) {
         if(x >= a.size.x || y >= a.size.y) {
            std::cerr << "array out of bounds" << x << y << "\n";
            return;
         }
         a(x,y) = v;
      }
   );

   lua.new_usertype<Array3Df>(
      "Array3D",

      sol::constructors<Array3Df()>(),

      "size", &Array3Df::size,

      "resize", &Array3Df::resize,

      "index", &Array3Df::index,

      "get", static_cast<float&(Array3Df::*)(int, int, int)>(&Array3Df::operator()),

      "set", [](Array3Df& a, int x, int y, int z, float v) {
         if(x >= a.size.x || y >= a.size.y || z >= a.size.z) {
            std::cerr << "array out of bounds" << x << y << z << "\n";
            return;
         }
         a(x, y, z) = v;
      }
   );

   lua.new_usertype<glm::ivec2>(
      "ivec2",

      sol::constructors<
         glm::ivec2(), 
         glm::ivec2(int, int)
      >(),
      "x", &glm::ivec2::x,
      "y", &glm::ivec2::y
   );

   lua.new_usertype<glm::ivec3>(
      "ivec3",
      sol::constructors<
         glm::ivec3(),
         glm::ivec3(int, int, int)
      >(),

      "x", &glm::ivec3::x,
      "y", &glm::ivec3::y,
      "z", &glm::ivec3::z
   );

   lua.new_usertype<glm::vec2>(
      "vec2",

      sol::constructors<
         glm::vec2(), 
         glm::vec2(int, int)
      >(),
      "x", &glm::vec2::x,
      "y", &glm::vec2::y
   );

   lua.new_usertype<glm::vec3>(
      "vec3",
      sol::constructors<
         glm::vec3(),
         glm::vec3(float, float, float)
      >(),

      "x", &glm::vec3::x,
      "y", &glm::vec3::y,
      "z", &glm::vec3::z
   );

   lua["setNoise"] = [&](FastNoiseLite::NoiseType noiseType) { noise.SetNoiseType(noiseType); };
   lua.set_function("noise", sol::overload(
      [&](float x, float y, float z) { return noise.GetNoise(x, y, z); },
      [&](float x, float y) { return noise.GetNoise(x, y); }
   ));
   lua["createHeightMap"] = [api, engine, this](Array2Df array, glm::vec3 position, glm::vec3 scale, glm::vec2 uvScale, bool smooth = true) {
      std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(api);
      Generator::HeightMap generator;
      generator.createMesh(array, position, scale, uvScale, smooth);
      mesh->createMesh(generator.Vertices, generator.Indices, engine->materialInstances["triPlanar"].get());

      engine->meshes["heightMap"] = std::move(mesh);

      glm::mat4 transform(1.0f);
      transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
      RenderObject object(engine->meshes["heightMap"].get(), transform);
      object.flag = "heightObject";

      engine->objects.erase(
         std::remove_if(engine->objects.begin(), engine->objects.end(),
            [](const RenderObject& v) {
                  return v.flag == "heightObject";
            }),
         engine->objects.end()
      );
      
      engine->objects.push_back(object);
   };

   lua["createSurface"] = [api, engine, this](Array3Df array, glm::vec3 position, glm::vec3 scale, glm::vec2 uvScale, bool interpolation, bool smooth, float iso) {
      std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(api);
      Generator::SurfaceNets generator;
      generator.createMesh(array, position, scale, uvScale, interpolation, smooth, iso);
      mesh->createMesh(generator.Vertices, generator.Indices, engine->materialInstances["triPlanar"].get());

      engine->meshes["surface"] = std::move(mesh);

      glm::mat4 transform(1.0f);
      transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
      RenderObject object(engine->meshes["surface"].get(), transform);
      object.flag = "surfaceObject";

      engine->objects.erase(
         std::remove_if(engine->objects.begin(), engine->objects.end(),
            [](const RenderObject& v) {
                  return v.flag == "surfaceObject";
            }),
         engine->objects.end()
      );
      
      engine->objects.push_back(object);
   };

   lua["print"] = [](sol::variadic_args va) {
      for (auto v : va) {
         std::cout << v.get<sol::object>().as<std::string>() << " ";
      }
   };

   lua["println"] = [](sol::variadic_args va) {
      for (auto v : va) {
         std::cout << v.get<sol::object>().as<std::string>() << " ";
      }

      std::cout << "\n";
   };
}

bool LuaEngine::runScript(std::string scriptSrc) {

   sol::protected_function_result result = lua.safe_script(scriptSrc.c_str());

   if (!result.valid()) {
      sol::error err = result;
      std::cout << "Lua Error: " << err.what() << std::endl;
      return false;
   }

   return true;
}
