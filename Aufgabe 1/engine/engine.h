#ifndef ENGINE_H
#define ENGINE_H

#include "system.h"
#include <GLFW/glfw3.h>
#include "framebuffer.h"

// Engine 
class Engine {
private:
  System* system;
  Framebuffer framebuffer;
  double thisTime;
  double lastTime; 
  glm::vec4 clearColor;
public:
  GLFWwindow* window = nullptr;
  void start(System* system, int Width, int Height, const char* Title);
  void setClearColor(glm::vec4 ClearColor);
  void calcMatrix();
  void startFramebuffer();
  void drawFramebuffer();
  void update();
};

#endif
