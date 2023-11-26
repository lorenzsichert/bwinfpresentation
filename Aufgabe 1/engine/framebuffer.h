#include <vector>
#include <glm/glm.hpp>
#include "system.h"

class Framebuffer {
  System* system;
  unsigned int vao;
  unsigned int vbo;
  unsigned int tex;
  unsigned int fbo;
  unsigned int rbo;
  unsigned int shaderId;

  unsigned int modelMatrixUL;
  unsigned int viewMatrixUL;
  unsigned int projectionMatrixUL;

  unsigned int verticesCount;
  glm::vec4 clearColor;
public:
  bool depthTest = false;
  void init(System* system);
  void setClearColor(glm::vec4 ClearColor);
  void resize();
  void setupBufferObject();
  void start();
  void draw();
  ~Framebuffer();
};
