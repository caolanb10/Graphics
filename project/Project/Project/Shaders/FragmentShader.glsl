#version 330

in vec4 vCol;
in vec2 texCo;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specIntensity;
	float shine;
};

uniform Material mat;
uniform DirectionalLight dirLight;
uniform sampler2D theTex;
uniform vec3 camera;

void main()
{
	vec4 ambientColour = vec4(dirLight.colour, 1.0f) * dirLight.ambientIntensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(dirLight.direction)), 0.0f);

	vec4 diffuseColour = vec4(dirLight.colour, 1.0f) * dirLight.diffuseIntensity * diffuseFactor;
	vec4 specularColour = vec4(0, 0, 0, 0);

	vec3 L = normalize(dirLight.direction - FragPos);
	vec3 V = normalize(camera - FragPos);

	//If no diffuse, then no specular
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToCamera = normalize(camera - FragPos);
		vec3 reflectedVertex = normalize(reflect(dirLight.direction, normalize(Normal)));
		
		float specFactor = dot(fragToCamera, reflectedVertex);
		if(specFactor > 0.0f)
		{
			specFactor = pow(specFactor, mat.shine);
			specularColour = vec4((dirLight.colour * mat.specIntensity * specFactor), 1.0f);
		}
	}
	colour = texture(theTex, texCo) * (ambientColour + diffuseColour + specularColour);
}