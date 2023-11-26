#include <GL/glew.h>
#include "engine.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>



void Engine::start(System* System, int Width, int Height, const char* Title) {
  system = System;
  // Linux
  system->screenWidth = Width;
  system->screenHeight = Height;
  system->width = system->screenWidth/system->pixelRatio;
  system->height = system->screenHeight/system->pixelRatio;
  system->aspectRatio = (float)system->width/system->height;
  system->camera.zoom = 1;
  system->camera.position = glm::vec3(0,0,0);

  clearColor = glm::vec4(0,0,0,1);

  glewExperimental = true;
  if (!glfwInit()) {
    std::cout << "Error GLFW Init" << std::endl;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(system->screenWidth, system->screenHeight, Title, NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    std::cout << "Error GLFW Init" << std::endl;
  }
  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cout << "Error GLEW Init" << std::endl;
  }
  thisTime = glfwGetTime();


  framebuffer.init(system);
}

void Engine::setClearColor(glm::vec4 ClearColor) {
  clearColor = ClearColor;
  framebuffer.setClearColor(clearColor);
}

void Engine::update() {
  lastTime = thisTime;
  thisTime = glfwGetTime();
  system->deltaTime = thisTime-lastTime;
  int width;
  int height;
  glfwGetWindowSize(window, &width, &height);
  if (width != system->screenWidth || height != system->screenHeight) {
    system->screenWidth = width;
    system->screenHeight = height;
    system->width = system->screenWidth/2.f;
    system->height = system->screenHeight/2.f;
    system->aspectRatio = (float)system->width/system->height;
    framebuffer.resize();
  }
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void Engine::startFramebuffer() {
  framebuffer.start();
}

void Engine::drawFramebuffer() {
  framebuffer.draw();
}
