#include "font.h"
#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>

void loadFont(std::map<char, Character>& characters, const char *fileName, unsigned int fontSize) {
  FT_Library ft;
  FT_Face face;

  if (FT_Init_FreeType(&ft)) {
    std::cout << "Error: Could not Init FreeType!" << std::endl;
    return;
  }

  if (FT_New_Face(ft, fileName, 0, &face)) {
    std::cout << "Error: Could not Load Font!" << std::endl;
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, fontSize);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character ch;
    ch.textureId = texture; 
    ch.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    ch.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    ch.advance = face->glyph->advance.x;
    characters.insert(std::pair<char, Character>(c, ch));
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}
