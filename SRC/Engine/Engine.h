#pragma once
#include <memory>
#include <unordered_map>

#include "../GraphicsApi/GraphicsApi.h"
#include "../Display/Display.h"
#include "../Renderer/Material.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/RenderObject.h"

#include "LuaEngine.h"


class GLApi;

class Engine {
public:
   Engine();

   template<typename Display, typename GApi>
   void createWindow(glm::ivec2 windowSize, const std::string& windowName = "function visualizer") {
      displayMannager = std::make_unique<Display>();
      if(std::is_same_v<GApi, GLApi>) {
         displayMannager->setApi(DisplayMannagerI::GL_API);
         displayMannager->setVersion(2,1,0);
      }
      window.reset(displayMannager->createWindow(windowSize, windowName));
      api = std::make_unique<GApi>();
   }

   void loadAssets();
   void createAssets();
   void createInputCallbacks();
   void updateControls();
   void updateDelta();

   void initLoop();
   void runLoop();

   void runLuaScript();

   ~Engine();

   std::unique_ptr<DisplayMannagerI> displayMannager;
   std::unique_ptr<GraphicsApiI> api;
   std::unique_ptr<WindowI> window;

   //Assets:
   std::unordered_map<std::string, std::unique_ptr<ImageI>> images;
   std::unordered_map<std::string, std::unique_ptr<Material>> materials;
   std::unordered_map<std::string, std::unique_ptr<MaterialInstance>> materialInstances;
   std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;

   //Objects:
   std::vector<RenderObject> objects;

   //LuaEngine:
   std::unique_ptr<LuaEngine> lua;

   //Controls:
   bool keys[1024] = {false};
   bool mouseButtons[256] = {false};
   glm::vec2 lastMouse;
   glm::vec3 camPos{0, 50, 50};
   glm::vec3 camRotation{0, 0, 0};
   int variant = 0;

   float baseSpeed = 25.0f;
   float speed = 0.0f;

   double currentTime = 0;
   double timer = 0;
   double delta = 0;
};