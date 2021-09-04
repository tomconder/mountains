#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec3 vFragPos;
varying vec3 vNormal;
varying vec2 vTexCoord;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform bool hasNoTexture;

vec3 blinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 color) {
  // diffuse
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color;

  // specular
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
  vec3 specular = spec * color;

  // attenuation
  float distance = clamp(length(lightPos - fragPos), 1.0, 1.5);
  float attenuation = 1.0 / (distance * distance);

  return attenuation * (diffuse + specular);
}

void main() {
  vec3 color;

  if (hasNoTexture) {
    color = vec3(1.0, 1.0, 1.0);
  } else {
    color = texture2D(texture_diffuse1, vTexCoord).rgb;
  }

  // ambient
  vec3 ambient = ambientStrength * color;

  vec3 lighting = blinnPhong(normalize(vNormal), vFragPos, lightPos, color);
  color *= lighting;

  // gamma
  float gamma = 2.2;
  color = pow(color, vec3(1.0 / gamma));

  gl_FragColor = vec4(ambient + color, 1.0);
}
