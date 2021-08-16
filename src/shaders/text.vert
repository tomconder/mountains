#version 100

#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 vertex;

varying vec2 vTexCoord;

uniform mat4 proj;

void main() {
  vTexCoord = vertex.zw;
  gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);
}
