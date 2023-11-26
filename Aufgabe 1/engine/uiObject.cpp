#include "uiObject.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

void UIObject::draw() {
  glUseProgram(shaderId);

  glUniformMatrix4fv(modelMatrixUL,1, GL_FALSE, &modelMatrix[0][0]);
  glUniformMatrix4fv(viewMatrixUL,1, GL_FALSE, &system->screenMatrix[0][0]);
  glUniformMatrix4fv(projectionMatrixUL,1, GL_FALSE, &glm::mat4(1)[0][0]);
  glUniform4f(colorUL, color[0], color[1], color[2], color[3]);

  glBindVertexArray(vao);
  glDrawArrays(drawPattern, 0, verticesCount);
}
