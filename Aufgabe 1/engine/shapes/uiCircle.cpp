#include "uiCircle.h"
#include <GL/glew.h>
#include <cmath>
#include <vector>

void UICircle::init(System* System, float Radius, int EdgeCount) {
  radius = Radius;
  edgeCount = EdgeCount;
  std::vector<float> vertices;
  for (int i = 0; i < edgeCount; i++) {
    vertices.push_back(std::sin(((float)i/edgeCount)*3.14*2)*radius);
    vertices.push_back(std::cos(((float)i/edgeCount)*3.14*2)*radius);
    vertices.push_back(0);
  }

  object.init(System, vertices, GL_TRIANGLE_FAN);
}

void UICircle::setPosition(glm::vec3 Position) {
  object.setPosition(Position);
}

void UICircle::setScale(glm::vec3 Scale) {
  object.setScale(Scale);
}

void UICircle::setRotation(glm::vec3 Rotation) {
  object.setRotation(Rotation);
}

void UICircle::setColor(glm::vec4 Color) {
  object.setColor(Color);
}

void UICircle::draw() {
  object.draw();
}
