#ifndef OBJECT_H
#define OBJECT_H

#include "system.h"
#include <glm/glm.hpp>
#include <vector>

class Object {
protected:
  System* system;

  unsigned int vao;
  unsigned int vbo;
  unsigned int shaderId;

  unsigned int modelMatrixUL;
  unsigned int viewMatrixUL;
  unsigned int projectionMatrixUL;
  unsigned int colorUL;

  unsigned int drawPattern;
  int verticesCount;

  glm::mat4 mvp;
  glm::mat4 modelMatrix;
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  glm::vec4 color;
public:
  void init(System* System, std::vector<float>& vertices, unsigned int DrawPattern);
  void reloadVertices(std::vector<float>& vertices);

  void setPosition(glm::vec3 Position);
  void setScale(glm::vec3 Scale);
  void setRotation(glm::vec3 Rotation);
  void setColor(glm::vec4 Color);
  void calcMatrix();

  void draw();
  ~Object();
};

#endif
