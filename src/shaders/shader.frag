#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 vTexCoord;

uniform sampler2D ourTexture1;

void main() {
  // gl_FragColor = texture2D(ourTexture1, vTexCoord);
  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
