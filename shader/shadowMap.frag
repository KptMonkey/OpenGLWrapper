#version 330

out vec4 color_out;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform vec4 viewPos;
uniform vec4 Color;
uniform sampler2D shadowMap;
uniform vec3 lightPos;

float shadowCalculation(vec4 fragPosLightSource)
{
  vec3 projCoord = fragPosLightSource.xyz/fragPosLightSource.w;
  projCoord = projCoord * 0.5 + 0.5;
  float closestDepth = texture(shadowMap, projCoord.xy).r;
  float currentDepth = projCoord.z;
  float bias = 0.005;
  float shadow = (currentDepth - bias)> closestDepth ? 1.0 : 0.0;

  return shadow;
}

void main()
{
  float ambientConst = 0.5;
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  
  vec3 ambientColor = lightColor * ambientConst;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos-FragPos);
  float diff = max(dot(norm, lightDir),0.0);
  vec3 diffColor = diff * lightColor;

  float specularConst = 0.5;
  vec3 viewDir = normalize(viewPos.xyz - FragPos);
  vec3 refDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir,refDir),0.0),32);
  vec3 specular = specularConst * spec * lightColor;
   
  float shadow = shadowCalculation(FragPosLightSpace);

  vec3 lightedColor = (ambientColor + (1.0-shadow) *(specular+diffColor) ) * Color.xyz;
  color_out = vec4(lightedColor,1.0);


}
