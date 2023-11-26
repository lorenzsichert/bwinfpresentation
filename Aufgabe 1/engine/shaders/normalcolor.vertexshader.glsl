#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNrm;

uniform mat4 MVP;
uniform vec4 color;

out vec4 finalColor;

void main() {
  vec3 normal_cameraspace = normalize(vec4(vec4(aNrm,1)).xyz); 
  vec3 cameraVector = normalize(vec3(0, 0, 10));

  float cosTheta = dot( normal_cameraspace, cameraVector ) / 2.f;

  finalColor = vec4(color.xyz * 0.5 + color.xyz * cosTheta,1);
  gl_Position = MVP * vec4(aPos,1);
}
