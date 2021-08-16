#version 100

#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 position;
attribute vec2 texCoord;

varying vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  gl_Position = proj * view * model * vec4(position, 1.0);
  vTexCoord = vec2(texCoord.x, texCoord.y);
}
