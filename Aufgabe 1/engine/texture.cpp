#include "texture.h"
#include "common/loadShader.h"
#include "common/loadPNG.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>

void Texture::init(System* System, glm::vec2 TextureRect) {
  system = System;
  mvp = glm::mat4(1); 
  modelMatrix = glm::mat4(1);
  position = glm::vec3(0,0,0);
  scale = glm::vec3(1,1,1);
  rotation = glm::vec3(0,0,0);
  color = glm::vec4(1,1,1,1);
  textureRect = TextureRect;
  textureOffset = glm::vec2(0,0);
  textureScale = glm::vec2(1,1);

  float w = textureRect.x * 0.5f;
  float h = textureRect.y * 0.5f;

  std::vector<float> vertices = {
    -w, h, 0, 0,0,
    -w, -h, 0, 0,1,
    w, -h, 0, 1,1,
    -w, h, 0, 0,0, 
    w, -h, 0, 1,1,
    w, h, 0, 1,0,
  };

  verticesCount = vertices.size() / 5;

  shaderId = loadShaders("../../engine/shaders/texture.vertexshader.glsl", "../../engine/shaders/texture.fragmentshader.glsl");

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

  // Vertex Coordinates
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0); 

  // Texture Coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); 


  modelMatrixUL = glGetUniformLocation(shaderId,"modelMatrix");
  viewMatrixUL = glGetUniformLocation(shaderId,"viewMatrix");
  projectionMatrixUL = glGetUniformLocation(shaderId,"projectionMatrix");
  offsetUL = glGetUniformLocation(shaderId,"offset");
  scaleUL = glGetUniformLocation(shaderId,"scale");
  colorUL = glGetUniformLocation(shaderId, "color");
}

void Texture::loadFromFile(const char *fileName, unsigned int filterMode) { 
  std::vector<unsigned char> image; //the raw pixels
  std::vector<unsigned char> png;
  unsigned width, height;

  //load and decode
  unsigned error = lodepng::load_file(png, fileName);
  if(!error) error = lodepng::decode(image, width, height, png);

  //if there's an error, display it
  if(error) std::cout << "Error decoding Image " << error << ": " << lodepng_error_text(error) << std::endl;

  std::vector<float> pixels;
  for (int i = 0; i < image.size(); i++) {
    pixels.push_back((float)image[i]/255);
  }

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D,tex);
  glActiveTexture(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixels.data());
}

void Texture::setTex(unsigned int Tex) {
  tex = Tex;
}

void Texture::setTextureOffset(glm::vec2 TextureOffset) {
  textureOffset = TextureOffset/textureRect;
  textureOffset *= -1;
}

void Texture::setTextureScale(glm::vec2 TextureScale) {
  textureScale = TextureScale;
}

void Texture::draw() {
  //mvp = system->projectionMatrix * system->viewMatrix * modelMatrix;
  glBindVertexArray(vao);
  glUseProgram(shaderId);
  glBindTexture(GL_TEXTURE_2D, tex);
  glActiveTexture(GL_TEXTURE_2D);

  glUniformMatrix4fv(modelMatrixUL,1, GL_FALSE, &modelMatrix[0][0]);
  glUniformMatrix4fv(viewMatrixUL,1, GL_FALSE, &system->viewMatrix[0][0]);
  glUniformMatrix4fv(projectionMatrixUL,1, GL_FALSE, &system->projectionMatrix[0][0]);
  glUniform2f(offsetUL, textureOffset.x, textureOffset.y);
  glUniform2f(scaleUL, textureScale.x, textureScale.y);
  glUniform4f(colorUL, color[0], color[1], color[2], color[3]);

  glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}

Texture::~Texture() { 
  glDeleteProgram(shaderId);
  glBindVertexArray(vao);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1,&tex);
  glDeleteBuffers(1, &vao);
}
