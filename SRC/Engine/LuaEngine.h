#pragma once

#include <sol/sol.hpp>

#include "../GraphicsApi/GraphicsApi.h"

#include <FastNoiseLite.h>

class Engine;

class LuaEngine {
public:
   LuaEngine(GraphicsApiI* api, Engine* engine);

   bool runScript(std::string scriptSrc);

   sol::state lua;
   GraphicsApiI* api;
   Engine* engine;
   FastNoiseLite noise;
};