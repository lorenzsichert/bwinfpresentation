#version 330 core

uniform vec4 textColor;
uniform sampler2D tex;

in vec2 TexCoord;

void main() {
  vec4 sampled = vec4(1.0,1.0,1.0, texture(tex, TexCoord).r);
  gl_FragColor = textColor * sampled;
}
