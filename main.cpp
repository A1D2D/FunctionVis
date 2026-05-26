#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SRC/Display/GlfwDisplay/GlfwDisplay.h"
#include "SRC/Engine/Engine.h"
#include "SRC/GraphicsApi/OpenGlApi/GLApi.h"

extern "C" {
   __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
   __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

Engine engine;

int main() {
   engine.createWindow<GlfwDisplayMannager, GLApi>({1536, 863});
   engine.loadAssets();
   engine.createAssets();
   engine.createInputCallbacks();

   engine.runLoop();
}