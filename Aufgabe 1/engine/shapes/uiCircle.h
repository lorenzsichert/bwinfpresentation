#ifndef UICIRCLE_H
#define UICIRCLE_H

#include "../uiObject.h"
#include "../system.h"
#include <glm/glm.hpp>

class UICircle {
private:
  UIObject object;
  float radius;
  int edgeCount;
public:
  void init(System* System, float Radius, int EdgeCount);
  void setPosition(glm::vec3 Position);
  void setScale(glm::vec3 Scale);
  void setRotation(glm::vec3 Rotation);
  void setColor(glm::vec4 Color);
  void draw();
};

#endif
