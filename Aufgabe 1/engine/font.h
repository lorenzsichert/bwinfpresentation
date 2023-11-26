#ifndef FONT_H
#define FONT_H

#include <freetype2/ft2build.h>
#include <glm/glm.hpp>
#include <map>

struct Character {
  unsigned int textureId;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

// Load Font using FreeType
void loadFont(std::map<char, Character>& characters, const char* fileName, unsigned int fontSize);

// Delete Texture Ids
void deleteFont(std::map<char, Character>& characters);

#endif
