#include "framebuffer.h"
#include <fstream>
#include <vector>
#include "common/loadShader.h"
#include <GL/glew.h>
#include <iostream>

void Framebuffer::init(System* System) {
  clearColor = glm::vec4(0,0,0,1);
  system = System;

  std::vector<float> vertices = {
    -1,1,0, 0,1,
    1,1,0, 1,1,
    1,-1,0, 1,0, 
    -1,-1,0, 0,0,
  };

  verticesCount = vertices.size()/5;

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

  // Generate Framebuffer
  setupBufferObject();

  modelMatrixUL = glGetUniformLocation(shaderId,"modelMatrix");
  viewMatrixUL = glGetUniformLocation(shaderId,"viewMatrix");
  projectionMatrixUL = glGetUniformLocation(shaderId,"projectionMatrix");
}

void Framebuffer::setClearColor(glm::vec4 ClearColor) {
  clearColor = ClearColor;
}

void Framebuffer::resize() {
  glDeleteTextures(1,&tex);
  glDeleteFramebuffers(1,&fbo);
  setupBufferObject();
}

void Framebuffer::setupBufferObject() {
  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
  }
  glBindVertexArray(vao);

  // Generate FrameBuffer
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Generate Texture
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, system->width, system->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

  // Generate RenderBuffer
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, system->width, system->height); // use a single renderbuffer object for both a depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Framebuffer Error!" << std::endl;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

void Framebuffer::start() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, system->width, system->height);
  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.a);
}

void Framebuffer::draw() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, system->screenWidth, system->screenHeight);
  glm::mat4 mvp = glm::mat4(1);

  glBindVertexArray(vao);
  glUseProgram(shaderId);
  glBindTexture(GL_TEXTURE_2D, tex);
  glActiveTexture(GL_TEXTURE_2D);
  glUniformMatrix4fv(modelMatrixUL,1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(viewMatrixUL,1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(projectionMatrixUL,1, GL_FALSE, &mvp[0][0]);

  glDrawArrays(GL_TRIANGLE_FAN, 0, verticesCount);
}

Framebuffer::~Framebuffer() {
  glDeleteProgram(shaderId);
  glBindVertexArray(vao);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1,&tex);
  glDeleteFramebuffers(1,&fbo);
  glDeleteRenderbuffers(1, &rbo);
  glDeleteBuffers(1, &vao);
}
