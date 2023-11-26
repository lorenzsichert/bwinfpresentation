#include "text.h"
#include "common/loadShader.h"
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

void Text::init(System* System, std::string Text, std::map<char, Character>& Characters) {
  system = System;
  text = Text;
  characters = Characters;

  shaderId = loadShaders("../../engine/shaders/text.vertexshader.glsl", "../../engine/shaders/text.fragmentshader.glsl");

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  // Vertex Coordinates
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); 

  // Texture Coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); 


  colorUL = glGetUniformLocation(shaderId, "textColor");
  mvpUL = glGetUniformLocation(shaderId, "MVP");
  mvp = system->screenMatrix;
}

void Text::setTextStyle(TextStyle TextStyle) {
  textStyle = TextStyle;
}

void Text::setColor(glm::vec4 Color) {
  color = Color;
}

void Text::setPosition(glm::vec2 Position) {
  position = Position;
}

void Text::setText(std::string Text) {
  text = Text;
}

void Text::getTextPosition() {
  float length = 0;
  for (int i = 0; i < text.size()-1; i++) {
    length += (characters[text[i]].advance >> 6) * textStyle.scale;
  }
  length += (characters[text[text.size()-1]].bearing.x) * textStyle.scale;
  length += (characters[text[text.size()-1]].size.x) * textStyle.scale;

  textPosition.x = position.x - (length/2.f);
  textPosition.y = position.y;
}

void Text::draw() {
  mvp = system->screenMatrix;
  glBindVertexArray(vao);
  glUseProgram(shaderId);
  glActiveTexture(GL_TEXTURE_2D);
  glUniformMatrix4fv(mvpUL, 1, GL_FALSE, &mvp[0][0]);
  glUniform4f(colorUL, color[0], color[1], color[2], color[3]);


  getTextPosition();
  glm::vec2 pos = textPosition;
  for (int i = 0; i < text.size(); i++) {
    Character c = characters[text[i]];
    glBindTexture(GL_TEXTURE_2D, c.textureId);
    vertices.clear();

    glm::vec2 curPos;
    curPos.x = pos.x + (c.bearing.x * textStyle.scale);
    curPos.y = pos.y - ((c.size.y - c.bearing.y) * textStyle.scale);
    float w = c.size.x * textStyle.scale;
    float h = c.size.y * textStyle.scale;

    vertices = {
      curPos.x, curPos.y + h, 0, 0,
      curPos.x, curPos.y, 0, 1,
      curPos.x + w, curPos.y, 1, 1,
      curPos.x, curPos.y + h, 0, 0,
      curPos.x + w, curPos.y, 1, 1,
      curPos.x + w, curPos.y + h, 1, 0,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), &vertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    pos.x += (c.advance >> 6) * textStyle.scale;
  }
}
