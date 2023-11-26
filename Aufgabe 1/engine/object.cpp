#include "object.h"
#include "common/loadShader.h"
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

void Object::init(System *System, std::vector<float> &vertices, unsigned int DrawPattern) {
  system = System;
  mvp = glm::mat4(1); 
  modelMatrix = glm::mat4(1);
  position = glm::vec3(0,0,0);
  scale = glm::vec3(1,1,1);
  rotation = glm::vec3(0,0,0);
  color = glm::vec4(1,1,1,1);
  drawPattern = DrawPattern;
  verticesCount = vertices.size()/3;

  shaderId = loadShaders("../../engine/shaders/color.vertexshader.glsl", "../../engine/shaders/color.fragmentshader.glsl");

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  modelMatrixUL = glGetUniformLocation(shaderId,"modelMatrix");
  viewMatrixUL = glGetUniformLocation(shaderId,"viewMatrix");
  projectionMatrixUL = glGetUniformLocation(shaderId,"projectionMatrix");
  colorUL = glGetUniformLocation(shaderId, "color");
  glBindVertexArray(0);

  calcMatrix();
}

void Object::reloadVertices(std::vector<float> &vertices) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
  verticesCount = vertices.size()/3;
}

void Object::setPosition(glm::vec3 Position) {
  position = Position;
  calcMatrix();
}

void Object::setScale(glm::vec3 Scale) {
  scale = Scale;
  calcMatrix();
}

void Object::setRotation(glm::vec3 Rotation) {
  rotation = Rotation;
  calcMatrix();
}

void Object::setColor(glm::vec4 Color) {
  color = Color;
}

void Object::calcMatrix() {
  modelMatrix = 
    glm::translate(position) *
    glm::rotate(rotation.x, glm::vec3(1,0,0)) *
    glm::rotate(rotation.y, glm::vec3(0,1,0)) *
    glm::rotate(rotation.z, glm::vec3(0,0,1)) *
    glm::scale(scale);
}

void Object::draw() {
  //mvp = system->projectionMatrix * system->viewMatrix * modelMatrix;

  glUseProgram(shaderId);
  glUniformMatrix4fv(modelMatrixUL,1, GL_FALSE, &modelMatrix[0][0]);
  glUniformMatrix4fv(viewMatrixUL,1, GL_FALSE, &system->viewMatrix[0][0]);
  glUniformMatrix4fv(projectionMatrixUL,1, GL_FALSE, &system->projectionMatrix[0][0]);
  glUniform4f(colorUL, color[0], color[1], color[2], color[3]);

  glBindVertexArray(vao);
  glDrawArrays(drawPattern, 0, verticesCount);
}

Object::~Object() {
  glDeleteProgram(shaderId);
  glBindVertexArray(vao);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}
