#include "GlfwDisplay.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GlfwDisplayMannager::GlfwDisplayMannager() {
   init();
}

bool GlfwDisplayMannager::init() {
   if (!glfwInit()) {
      return false;
   }
#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
   return true;
}

void GlfwDisplayMannager::terminate() {
   glfwTerminate();
}

void GlfwDisplayMannager::setApi(DisplayApi apiID) {
   api = apiID;
}

void GlfwDisplayMannager::setVersion(unsigned int v1, unsigned int v2, unsigned int v3) {
   switch (api) {
   case DisplayApi::GL_API:
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, v2);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, v1);
      break;
   case DisplayApi::UNKNOWN:
   default:
      break;
   }
}

WindowI* GlfwDisplayMannager::createWindow(glm::ivec2 size, std::string title) {
   GLFWwindow* nativeWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
   if (!nativeWindow) return nullptr;
   GlfwWindow* window = new GlfwWindow;
   window->nativeWindow = nativeWindow;
   window->size = size;

   glfwSetWindowUserPointer(window->nativeWindow, window);

   glfwSetKeyCallback(window->nativeWindow, GlfwWindow::s_keyCallback);
   glfwSetMouseButtonCallback(window->nativeWindow, GlfwWindow::s_mouseBtnCallback);
   glfwSetCursorPosCallback(window->nativeWindow, GlfwWindow::s_cursorPosCallback);
   glfwSetScrollCallback(window->nativeWindow, GlfwWindow::s_scrollCallback);

   switch (api) {
   case DisplayApi::GL_API:
      glfwMakeContextCurrent(nativeWindow);
      break;
   case DisplayApi::UNKNOWN:
   default:
      break;
   }
   return window;
}

GlfwDisplayMannager::~GlfwDisplayMannager() {
   terminate();
}


void GlfwWindow::pollEvents() {
   glfwPollEvents();
}

bool GlfwWindow::isClosed() {
   return glfwWindowShouldClose(nativeWindow);
}

glm::ivec2 GlfwWindow::getSize() {
   glfwGetWindowSize(nativeWindow, &size.x, &size.y);
   return size;
}

void GlfwWindow::setSize(glm::ivec2 size) {
   this->size = size;
   glfwSetWindowSize(nativeWindow, size.x, size.y);
}

void GlfwWindow::swapBuffers() {
   glfwSwapBuffers(nativeWindow);
}

void GlfwWindow::s_keyCallback(GLFWwindow* w, int key, int scan, int action, int mods) {
   auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(w));
   if (self && self->keyCb) self->keyCb(key, scan, action, mods);
}

void GlfwWindow::s_mouseBtnCallback(GLFWwindow* w, int btn, int action, int mods) {
   auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(w));
   if (self && self->mouseBtnCb) self->mouseBtnCb(btn, action, mods);
}

void GlfwWindow::s_cursorPosCallback(GLFWwindow* w, double x, double y) {
   auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(w));
   if (self && self->cursorCb) self->cursorCb(x, y);
}

void GlfwWindow::s_scrollCallback(GLFWwindow* w, double xOff, double yOff) {
   auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(w));
   if (self && self->scrollCb) self->scrollCb(xOff, yOff);
}

GlfwWindow::~GlfwWindow() {
   glfwDestroyWindow(nativeWindow);
}