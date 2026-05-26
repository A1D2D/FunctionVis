#include "Engine.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#include "../Loaders/FileLoader.h"
#include "../Loaders/ImageLoader.h"
#include "../Loaders/ObjLoader.h"

#include "../Display/GlfwDisplay/GlfwDisplay.h"
#include "LuaEngine.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Engine::Engine() {}

void Engine::loadAssets() {
   std::vector<std::string> texturePaths = {
      "Blocky_D.png",
      "Crate1_N.png",
      "Crate1.png",
      "UVTest.jpg",
      "grid.png"
   };

   struct ShaderPath {
      std::string vert;
      std::string frag;
   };

   std::vector<ShaderPath> shaderPaths = {
      {"uv_vert.glsl", "uv_frag.glsl"},
      {"triPlanar_vert.glsl", "triPlanar_frag.glsl"}
   };

   std::cout << "Loading Textures..\n";
   ImageLoader::Loader imageLoader;
   for (auto& texturePath : texturePaths) {
      std::string imageSrc = FileLoader::loadFile(texturePath);
      std::string fileName = std::filesystem::path(texturePath).stem().string();

      if(!imageLoader.loadFromSrc(imageSrc)) {
         std::cerr << "failed to load image\n";
      }
      std::unique_ptr<ImageI> image(api->createImage());
      image->setChannel(0);
      image->bind();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      image->setData(imageLoader.data, imageLoader.size, GraphicsApiI::ApiDataType::UBYTE, GraphicsApiI::TexFormatType::RGBA, 0, 
         imageLoader.nrChannels == 4 ? GraphicsApiI::TexFormatType::RGBA : GraphicsApiI::TexFormatType::RGB);
      image->genMips();

      images[fileName] = std::move(image);
   }

   std::cout << "Compiling Shaders/Materials..\n";
   for (auto& shaderPath : shaderPaths) {
      std::string vertShaderSrc = FileLoader::loadFile(shaderPath.vert);
      std::string fragShaderSrc = FileLoader::loadFile(shaderPath.frag);
      std::string fileName = std::filesystem::path(shaderPath.frag).stem().string();

      int fragPos = fileName.find("_frag");
      if(fragPos != std::string::npos) fileName.erase(fragPos, 5);

      std::unique_ptr<Material> material = std::make_unique<Material>(api.get());
      material->createMaterial(vertShaderSrc, fragShaderSrc);

      materials[fileName] = std::move(material);
   }
}

void Engine::createAssets() {
   std::unique_ptr<MaterialInstance> uvMat = std::make_unique<MaterialInstance>(materials["triPlanar"].get());
   uvMat->setSampler({"tex", images["UVTest"].get()});
   materialInstances["triPlanar"] = std::move(uvMat);

   std::unique_ptr<MaterialInstance> gridMat = std::make_unique<MaterialInstance>(materials["uv"].get());
   gridMat->setSampler({"tex", images["grid"].get()});
   materialInstances["grid"] = std::move(gridMat);

   //Grid:
   std::string plane = FileLoader::loadFile("plane.obj");

   OBJL::Loader objLoader;
   objLoader.LoadFromSRC(plane);

   std::unique_ptr<Mesh> planeMesh = std::make_unique<Mesh>(api.get());
   planeMesh->createMesh(objLoader.LoadedVertices, objLoader.LoadedIndices, materialInstances["grid"].get());

   meshes["plane"] = std::move(planeMesh);

   glm::mat4 planeTransform(1.0f);
   planeTransform = glm::scale(planeTransform, glm::vec3(10, 10, 10));
   RenderObject planeObject(meshes["plane"].get(), planeTransform);
   objects.push_back(planeObject);
}

void Engine::createInputCallbacks() {
   //Controll:
   window->setKeyCallback([this](int key, int scancode, int action, int mods) {
      if(action == GLFW_PRESS) {
         keys[key] = true;

         switch (key) {
            case GLFW_KEY_M:
               variant++;
               break;
            case GLFW_KEY_R:
               runLuaScript();
               break;
            default:
               break;
         }
      } else if(action == GLFW_RELEASE) {
         keys[key] = false;
      }
   });

   // glfwSetInputMode(((GlfwWindow*)window.get())->nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   
   window->setMouseBtnCallback([this](int button, int action, int mods) {
      if(action == GLFW_PRESS) {
         mouseButtons[button] = true;
      } else if(action == GLFW_RELEASE) {
         mouseButtons[button] = false;
      }
   });

   window->setCursorPosCallback([this](double mouseX, double mouseY) {
      glm::vec2 mousePos(mouseX, mouseY);
      glm::vec2 dif = mousePos - lastMouse;
      lastMouse = mousePos;
      if(mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
         camRotation.y += dif.x*0.2f;
         camRotation.x += dif.y*0.2f;
         camRotation.x = std::max(std::min(camRotation.x, 90.0f), -90.0f);
      }
   });
}

void Engine::updateControls() {
   speed = baseSpeed;
   if(keys[GLFW_KEY_LEFT_SHIFT]) speed = baseSpeed*10.0f;
   float forward = 0.0f;
   if(keys[GLFW_KEY_W]) forward = speed;
   if(keys[GLFW_KEY_S]) forward = -speed;

   float side = 0.0f;
   if(keys[GLFW_KEY_A]) side = -speed;
   if(keys[GLFW_KEY_D]) side = speed;

   camPos.x += sin(glm::radians(camRotation.y)) * forward * delta + cos(glm::radians(-camRotation.y)) * side * delta;
   camPos.z -= cos(glm::radians(camRotation.y)) * forward * delta + sin(glm::radians(-camRotation.y)) * side * delta;

   if(keys[GLFW_KEY_SPACE]) camPos.y += speed * delta;
   if(keys[GLFW_KEY_LEFT_CONTROL]) camPos.y -= speed * delta;

   if(keys[GLFW_KEY_V]) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   } else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   if(keys[GLFW_KEY_B]) {
      glDisable(GL_CULL_FACE);
   } else glEnable(GL_CULL_FACE);

   if(keys[GLFW_KEY_H]) {
      camPos = {0, 50, 50};
      camRotation = {0, 0, 0};
   }

}

void Engine::updateDelta() {
   double lastTime = currentTime;
   currentTime = glfwGetTime();

   delta = (currentTime - lastTime);

   timer += delta;
   if (timer >= 0.5) {
      double fps = 1.0 / delta;

      std::string title =
         "FPS: " + std::to_string((int)fps) +
         " | MS: " + std::to_string(delta*1000.0f);

      glfwSetWindowTitle(((GlfwWindow*)window.get())->nativeWindow, title.c_str());
      timer = 0;
   }
}

void Engine::initLoop() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   glfwSwapInterval(0);
}

void Engine::runLoop() {
   initLoop();
   timer = 0.0f;
   currentTime = glfwGetTime();

   materialInstances["grid"]->setUniform({"renderVariant", 3});

   while(!window->isClosed()) {
      updateDelta();
      updateControls();
      
      glm::ivec2 winSize = window->getSize();
      if(winSize.x <= 0 || winSize.y <= 0) winSize = glm::ivec2(1, 1);
      api->setViewport({0, 0}, winSize);
      api->clearColor({0.2,0.2,0.2,1});
      api->clear(GraphicsApiI::ClearType::COLOR | GraphicsApiI::ClearType::DEPTH);

      glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)winSize.x/(float)winSize.y, 0.1f, 1000.0f);

      glm::mat4 view = glm::mat4(1.0f);
      view = glm::rotate(view, glm::radians(camRotation.z), glm::vec3(0, 0, 1.0f));
      view = glm::rotate(view, glm::radians(camRotation.x), glm::vec3(1.0f, 0, 0));
      view = glm::rotate(view, glm::radians(camRotation.y), glm::vec3(0, 1.0f, 0));
      view = glm::translate(view, glm::vec3(0)-camPos);

      materials["triPlanar"]->setProjMatrix(proj);
      materials["triPlanar"]->setViewMatrix(view);

      materials["uv"]->setProjMatrix(proj);
      materials["uv"]->setViewMatrix(view);

      materialInstances["triPlanar"]->setUniform({"renderVariant", variant % 3});

      for (int i = 0; i < objects.size(); i++) {
         objects[i].render();
      }

      window->swapBuffers();
      window->pollEvents();
   }
}

void Engine::runLuaScript() {
   lua = std::make_unique<LuaEngine>(api.get(), this);
   std::string scriptSrc = FileLoader::loadFile("script.lua");
   if(scriptSrc.empty()) std::cerr << "scriptEmpty\n";
   lua->runScript(scriptSrc);
}

Engine::~Engine() {}
