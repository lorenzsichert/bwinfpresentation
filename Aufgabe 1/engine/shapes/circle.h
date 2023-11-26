#ifndef CIRCLE_H
#define CIRCLE_H

#include "../object.h"
#include "../system.h"
#include <glm/glm.hpp>

class Circle {
private:
  Object object;
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
