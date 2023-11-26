#ifndef TEXTURE_H
#define TEXTURE_H

#include "object.h"
#include "system.h"
#include <glm/glm.hpp>
class Texture : public Object{
protected:
  unsigned int tex;

  unsigned int offsetUL;
  unsigned int scaleUL;

  glm::vec2 textureRect;
  glm::vec2 textureOffset;
  glm::vec2 textureScale;


public:
  void init(System* System, glm::vec2 TextureRect);
  void loadFromFile(const char* fileName, unsigned int filterMode);
  void setTex(unsigned int Tex);

  void setTextureOffset(glm::vec2 TextureOffset);
  void setTextureScale(glm::vec2 TextureScale);
  
  void draw();
  ~Texture();
};

#endif
