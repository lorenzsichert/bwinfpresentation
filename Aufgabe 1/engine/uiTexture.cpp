#include "uiTexture.h"

#include <GL/glew.h>

void UITexture::draw() {
  glBindVertexArray(vao);
  glUseProgram(shaderId);
  glBindTexture(GL_TEXTURE_2D, tex);
  glActiveTexture(GL_TEXTURE_2D);

  glUniformMatrix4fv(modelMatrixUL,1, GL_FALSE, &modelMatrix[0][0]);
  glUniformMatrix4fv(viewMatrixUL,1, GL_FALSE, &glm::mat4(1)[0][0]);
  glUniformMatrix4fv(projectionMatrixUL,1, GL_FALSE, &system->screenMatrix[0][0]);
  glUniform2f(offsetUL, textureOffset.x, textureOffset.y);
  glUniform2f(scaleUL, textureScale.x, textureScale.y);
  glUniform4f(colorUL, color[0], color[1], color[2], color[3]);

  glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}
