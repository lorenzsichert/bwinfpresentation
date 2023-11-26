#version 330 core

in vec2 Texcoord;

uniform sampler2D tex;
uniform float scale;
uniform vec2 offset;
uniform float alpha;
uniform float parralax;

void main() {
  gl_FragColor = texture(tex, (Texcoord+(offset*parralax))*scale) - vec4(alpha,alpha,alpha,0);
}
