#pragma once
#include <glm/glm.hpp>
#include <string>
#include <functional>

class WindowI;

class DisplayMannagerI {
public:
   enum DisplayApi {
      UNKNOWN,
      GL_API
   };

   virtual bool init() = 0;
   virtual void terminate() = 0;
   virtual void setApi(DisplayApi apiID) = 0;
   virtual void setVersion(unsigned int v1, unsigned int v2, unsigned int v3) = 0;
   virtual WindowI* createWindow(glm::ivec2 size, std::string windowName = "Window") = 0;
   virtual ~DisplayMannagerI() = default;
};

using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
using MouseBtnCallback = std::function<void(int button, int action, int mods)>;
using CursorPosCallback = std::function<void(double x, double y)>;
using ScrollCallback = std::function<void(double xOff, double yOff)>;

class WindowI {
public: 
   virtual ~WindowI() = default;
   virtual void pollEvents() = 0;
   virtual bool isClosed() = 0;
   virtual glm::ivec2 getSize() = 0;
   virtual void setSize(glm::ivec2 size) = 0;
   virtual void swapBuffers() = 0;

   virtual void setKeyCallback(KeyCallback cb) = 0;
   virtual void setMouseBtnCallback(MouseBtnCallback cb) = 0;
   virtual void setCursorPosCallback(CursorPosCallback cb) = 0;
   virtual void setScrollCallback(ScrollCallback cb) = 0;
};