#version 330

out vec4 color_out;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec2 TexPos;
in vec3 worldTangent;

uniform vec4 viewPos;
uniform vec4 Color;
uniform sampler2D shadowMap;
uniform sampler2D ground;
uniform sampler2D groundNormal;
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

  
  float ambientConst = 0.1;
  vec3 color = vec3(texture(ground,TexPos));
  vec3 ambientColor = color * ambientConst;

  vec3 norm = normalize(Normal);

  //Normal Mapping
  norm = texture(groundNormal,TexPos).rgb;
  norm = normalize(norm*2.0 -1.0);

//  vec3 Tangent = normalize(worldTangent);
//  Tangent = normalize( Tangent - dot(Tangent,norm) * norm);
//  vec3 BitTangent = cross(Tangent,norm);
//  vec3 bumpMapNormal = texture(groundNormal,TexPos).xyz;
//  bumpMapNormal = bumpMapNormal*2.0 - vec3(1.0);
//  vec3 newNormal;
//  mat3 TBN = mat3(Tangent, BitTangent, norm);
//  newNormal = TBN * bumpMapNormal;
//  newNormal = normalize(newNormal);
//  norm = newNormal;


  vec3 lightDir = normalize(lightPos-FragPos);
  float diff = max(dot(norm, lightDir),0.0);
  vec3 diffColor = diff * color;

  float specularConst = 0.2;
  vec3 viewDir = normalize(viewPos.xyz - FragPos);
  vec3 refDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir,refDir),0.0),32);
  vec3 specular = specularConst * spec * color;
   
  float shadow = shadowCalculation(FragPosLightSpace);

  vec3 lightedColor = ambientColor + (1.0-shadow) *(specular+diffColor) * color;
  color_out =vec4(lightedColor,1.0);



}
