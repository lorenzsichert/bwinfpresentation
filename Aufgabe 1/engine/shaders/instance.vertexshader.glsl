#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 aMVP;
layout (location = 5) in vec4 aCol;


out vec4 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
  color = aCol;
  gl_Position = projectionMatrix * viewMatrix *  modelMatrix * aMVP * vec4(aPos,1);
}
