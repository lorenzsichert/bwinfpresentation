#version 330 core

in vec2 Texcoord;

uniform sampler2D tex;
uniform vec2 offset = vec2(0,0);
uniform vec2 scale = vec2(1,1);
uniform vec4 color = vec4(1,1,1,1); 

void main() {
  gl_FragColor = texture(tex, scale*(offset+Texcoord)) * color;
}
