#version 330 core

in vec2 UV;
in vec3 normal;
in vec3 fragPosition;
in vec4 vertexColor;

uniform vec3 u_color;
uniform vec3 u_light_position;
uniform vec3 u_light_direction;
uniform vec3 u_camera_position;
uniform vec3 u_ambient_color;
uniform vec3 u_light_color;
uniform sampler2D texture_diffuse;

out vec4 FragColor;

float attenuate(float value, float minimum, float maximum) {
	return 1.0 - (clamp(value, minimum, maximum) - minimum) / (maximum - minimum);
}

float simple(float value, float maximum) {
	float clampVal;
	if (value > maximum) clampVal = 0.0;
	else clampVal = 1.0;
	return clampVal;
}

void main()
{
	vec3 lightDirection = normalize(u_light_position - fragPosition);
	vec3 lightFocus = normalize(-u_light_direction);
	float lightDistance = length(u_light_position - fragPosition);

	float specularStrength = 1.0f;
	vec3 viewDirection = normalize(u_camera_position - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(reflectDirection, viewDirection), 0.0f), 16);

	vec3 specular = specularStrength * spec * u_light_color;
	vec3 diffuse = vec3(max(dot(normal, lightDirection), 0.0)) * u_light_color;
	vec3 ambient = u_ambient_color * u_light_color;

	float theta = abs(acos(dot(lightDirection, lightFocus)));
	float gradient = attenuate(theta, 0.01, 0.02);

	FragColor = vec4(ambient + gradient * (diffuse + specular), 1.0) * texture(texture_diffuse, UV);
}