#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec3 vFragPos;
varying vec3 vNormal;
varying vec2 vTexCoord;

uniform sampler2D image;

void main() {
  vec3 color = texture(image, vTexCoord);
  vec3 lightPos = vec3(40.0, 40.0, 40.0);

  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * color;

  vec3 norm = normalize(vNormal);
  vec3 lightDir = normalize(lightPos - vFragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * color;

  gl_FragColor = vec4(ambient + diffuse, 1.0);
}
