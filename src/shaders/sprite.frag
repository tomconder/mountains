#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 vTexCoord;

uniform sampler2D image;

void main() {
  gl_FragColor = texture2D(image, vTexCoord);
}
