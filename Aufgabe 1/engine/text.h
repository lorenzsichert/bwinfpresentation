#ifndef TEXT_H
#define TEXT_H

#include "system.h"
#include <glm/fwd.hpp>
#include <map>
#include <string>
#include <vector>
#include "font.h"

struct TextStyle {
  bool centered = false;
  bool lineBreak = false;
  float lineBreakThreshold = 0;
  float scale = 0.1;
};

class Text {
  System* system;
  std::map<char, Character> characters;
  std::vector<float> vertices;

  unsigned int vao;
  unsigned int vbo;
  unsigned int shaderId;
  unsigned int mvpUL;
  unsigned int colorUL;

  TextStyle textStyle;

  std::string text;
  glm::vec2 position;
  glm::vec2 textPosition;
  glm::mat4 mvp;
  glm::vec4 color;


public:
  void init(System* System, std::string Text, std::map<char, Character>& Characters);
  void setTextStyle(TextStyle TextStyle);
  void setColor(glm::vec4 Color);
  void setPosition(glm::vec2 Position);
  void setText(std::string Text);
  void getTextPosition();
  void draw();
};

#endif
