#version 330 core
out vec4 color;
in vec3 pos;
in vec2 TexPos;
uniform sampler2D terrain;
void main()
{
  color = vec4(vec3(texture(terrain,TexPos)),1.0);

}
