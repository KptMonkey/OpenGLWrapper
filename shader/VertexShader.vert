#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec2 texPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 pos;
out vec2 TexPos;

void main()
{
        gl_Position = projection*view*model*  vec4(position,1.0);
        pos = position;
        TexPos = texPos;
}
