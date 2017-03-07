#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texPos;
layout(location=3) in vec3 tangent;

out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;
out vec2 TexPos;
out vec3 worldTangent;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpace;

void main()
{
	gl_Position = projection * view * model * vec4(position,1.0);
        FragPos = vec3(model*vec4(position,1.0));
        Normal = normal;
	TexPos = texPos;
        worldTangent = vec3(model * vec4(tangent,1.0));
        FragPosLightSpace = lightSpace * vec4(FragPos,1.0);
}
