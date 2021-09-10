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

float attenuate(float value, float maximum) {
	float clampVal = min(value, maximum);
	return 1.0 / (pow(5 * clampVal / maximum, 2) + 1);
}

void main()
{
	vec3 lightDirection = normalize(u_light_position - fragPosition);
	float lightDistance = length(u_light_position - fragPosition);

	float specularStrength = 1.0f;
	vec3 viewDirection = normalize(u_camera_position - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(reflectDirection, viewDirection), 0.0f), 16);

	vec3 specular = specularStrength * spec * u_light_color;
	vec3 diffuse = vec3(max(dot(normal, lightDirection), 0.0)) * u_light_color;
	vec3 ambient = u_ambient_color * u_light_color;
	float gradient = attenuate(lightDistance, 100.0f);

	FragColor = vec4(ambient + gradient * (diffuse + specular), 1.0) * texture(texture_diffuse, UV);
}