#version 330

in vec3 texCo;

out vec4 colour;

uniform samplerCube skyBox;

void main()
{
	colour = texture(skyBox, texCo);
}