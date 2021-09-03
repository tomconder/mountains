#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec3 vFragPos;
varying vec3 vNormal;
varying vec2 vTexCoord;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;

void main() {
  vec3 color = texture2D(texture_diffuse1, vTexCoord).rgb;

  // ambient
  float ambientStrength = 0.63;
  vec3 ambient = ambientStrength * color;

  // diffuse
  vec3 lightDir = normalize(lightPos - vFragPos);
  vec3 normal = normalize(vNormal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color;

  gl_FragColor = vec4(ambient + diffuse, 1.0);
}
