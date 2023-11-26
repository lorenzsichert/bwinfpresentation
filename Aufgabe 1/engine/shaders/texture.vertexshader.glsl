#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 Texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
  Texcoord = aTex;
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1);
}
