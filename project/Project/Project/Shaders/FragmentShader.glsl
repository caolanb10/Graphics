#version 330

in vec2 texCo;
in vec4 vCol;
out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;

};
uniform DirectionalLight dirLight;
uniform sampler2D theTex;

void main()
{
	vec4 ambientColour = vec4(dirLight.colour, 1.0f) * dirLight.ambientIntensity;
	colour = texture(theTex, texCo) * ambientColour;
}