#version 330 core
out vec4 color;
in  vec2 texPos;
uniform sampler2D lambda;
void main()
{
  color = texture(lambda,texPos);

}
