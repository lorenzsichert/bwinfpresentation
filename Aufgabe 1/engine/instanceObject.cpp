#include "instanceObject.h"
#include <GL/glew.h>
#include <cstddef>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>
#include "common/loadShader.h"

void InstanceObject::init(System *System, std::vector<float> &vertices, int Count, unsigned int DrawPattern) {
  system = System;

  position = glm::vec3(0,0,0);
  rotation = glm::vec3(0,0,0);
  scale = glm::vec3(1,1,1);
  color = glm::vec4(1,1,1,1);
  mvp = glm::mat4(1);
  modelMatrix = glm::mat4(1);

  verticesCount = vertices.size()/3;
  count = Count;
  drawPattern = DrawPattern;

  mvps.resize(count);

  for (int i = 0; i < mvps.size(); i++) {
    mvps[i] = glm::mat4(1);
  }

  shaderId = loadShaders("../../engine/shaders/instance.vertexshader.glsl", "../../engine/shaders/instance.fragmentshader.glsl");

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() , &vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
  glVertexAttribDivisor(0,0);

  glGenBuffers(1, &mbo);
  glBindBuffer(GL_ARRAY_BUFFER, mbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mvps.size() , &mvps[0], GL_STATIC_DRAW);

  std::size_t vec4Size = sizeof(glm::vec4);
  glEnableVertexAttribArray(1); 
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
  glEnableVertexAttribArray(2); 
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
  glEnableVertexAttribArray(3); 
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
  glEnableVertexAttribArray(4); 
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

  glVertexAttribDivisor(1, 1);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);

  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colors.size() , NULL, GL_STATIC_DRAW);
  glEnableVertexAttribArray(5); 
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
  glVertexAttribDivisor(5, 1);

  glBindVertexArray(0);
 
  calcMatrix();
  modelMatrixUL = glGetUniformLocation(shaderId,"modelMatrix");
  viewMatrixUL = glGetUniformLocation(shaderId,"viewMatrix");
  projectionMatrixUL = glGetUniformLocation(shaderId,"projectionMatrix");
  colorUL = glGetUniformLocation(shaderId, "color");
}

void InstanceObject::setInstancePositions(std::vector<glm::vec3> &Positions) {
  positions = Positions;
}

void InstanceObject::setInstanceScales(std::vector<glm::vec3> &Scales) {
  scales = Scales;
}

void InstanceObject::setInstanceRotations(std::vector<glm::vec3> &Rotations) {
  rotations = Rotations;
}

void InstanceObject::setInstanceColors(std::vector<glm::vec4> &Colors) {
  colors = Colors;
}

void InstanceObject::calcInstanceMVPs() {
  for (int i = 0; i < mvps.size(); i++) {
    mvps[i] = glm::mat4(1);
    mvps[i] = glm::translate(mvps[i], positions[i]);
    if (rotations[i].z != 0) {
      mvps[i] = glm::rotate(mvps[i], rotations[i].z, glm::vec3(0,0,1));
    }
    mvps[i] = glm::scale(mvps[i],scales[i]);
  }
}

void InstanceObject::updateBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, mbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mvps.size() , &mvps[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colors.size(), &colors[0], GL_STATIC_DRAW);
}

void InstanceObject::draw() { 
  calcMatrix();
  calcInstanceMVPs();
  updateBuffers();

  glUseProgram(shaderId);
  glUniformMatrix4fv(modelMatrixUL,1, GL_FALSE, &modelMatrix[0][0]);
  glUniformMatrix4fv(viewMatrixUL,1, GL_FALSE, &system->viewMatrix[0][0]);
  glUniformMatrix4fv(projectionMatrixUL,1, GL_FALSE, &system->projectionMatrix[0][0]);
  glActiveTexture(GL_TEXTURE_2D);

  glBindVertexArray(vao);
  glDrawArraysInstanced(drawPattern, 0, verticesCount, count);
}

InstanceObject::~InstanceObject() {
  glDeleteProgram(shaderId);
  glBindVertexArray(vao);
  glDisableVertexAttribArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &mbo);
  glDeleteVertexArrays(1, &vao);
}
