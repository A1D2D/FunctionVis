#pragma once
#include "../Display.h"

class GLFWwindow;

class GlfwDisplayMannager : public DisplayMannagerI {
public:
   GlfwDisplayMannager();
   bool init() override;
   void terminate() override;
   WindowI* createWindow(glm::ivec2 size, std::string title = "window") override;
   void setApi(DisplayApi apiID = DisplayApi::GL_API) override;
   void setVersion(unsigned int v1, unsigned int v2, unsigned int v3) override;
   ~GlfwDisplayMannager() override;
   DisplayApi api = DisplayApi::GL_API;
};

class GlfwWindow : public WindowI {
public:
   GLFWwindow* nativeWindow = nullptr;
   glm::ivec2 size;

   void pollEvents() override;
   bool isClosed() override;
   glm::ivec2 getSize() override;
   void setSize(glm::ivec2 size) override;
   void swapBuffers() override;

   void setKeyCallback(KeyCallback cb) override { keyCb = std::move(cb); }
   void setMouseBtnCallback(MouseBtnCallback cb) override { mouseBtnCb = std::move(cb); }
   void setCursorPosCallback(CursorPosCallback cb) override { cursorCb = std::move(cb); }
   void setScrollCallback(ScrollCallback cb) override { scrollCb = std::move(cb); }

   ~GlfwWindow() override;

private:
   KeyCallback keyCb;
   MouseBtnCallback mouseBtnCb;
   CursorPosCallback cursorCb;
   ScrollCallback scrollCb;

   static void s_keyCallback(GLFWwindow* w, int key, int scan, int action, int mods);
   static void s_mouseBtnCallback(GLFWwindow* w, int btn, int action, int mods);
   static void s_cursorPosCallback(GLFWwindow* w, double x, double y);
   static void s_scrollCallback(GLFWwindow* w, double xOff, double yOff);
   friend class GlfwDisplayMannager;
};