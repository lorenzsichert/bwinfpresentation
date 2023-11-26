#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 Texcoord;

uniform mat4 MVP;

void main() {
  Texcoord = aPos;
  gl_Position = MVP * vec4(aPos, 0, 1);
}
