#ifndef SYSTEM_H
#define SYSTEM_H

#include <glm/glm.hpp>
#include "../gameState.h"

struct Camera {
  // Forced
  glm::vec3 position;
  float zoom = 1;
};

struct System {
  // Forced
  int screenWidth;
  int screenHeight;
  int width;
  int height; 
  float pixelRatio = 2;
  int unit = 1000;
  float aspectRatio;
  Camera camera;
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 screenMatrix;
  float deltaTime;

  GameState gameState;

  glm::vec2 mousePos;
  glm::vec2 mouseScreenPos;
};

#endif
