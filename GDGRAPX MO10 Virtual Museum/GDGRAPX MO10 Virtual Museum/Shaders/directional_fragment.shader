#version 330 core

in vec2 UV;
in vec3 normal;
in vec3 fragPosition;
in vec4 vertexColor;

uniform vec3 u_color;
uniform vec3 u_light_direction;
uniform vec3 u_camera_position;
uniform vec3 u_ambient_color;
uniform vec3 u_light_color;
uniform sampler2D texture_diffuse;

out vec4 FragColor;

void main()
{
	vec3 lightDirection = normalize(u_light_direction);
	vec3 ambient = u_ambient_color * u_light_color;

	if (lightDirection.y < 0) FragColor = vec4(ambient, 1.0) * texture(texture_diffuse, UV);
	else {
		float specularStrength = 1.0f;
		vec3 viewDirection = normalize(u_camera_position - fragPosition);
		vec3 reflectDirection = reflect(-lightDirection, normal);
		float spec = pow(max(dot(reflectDirection, viewDirection), 0.0f), 16);

		vec3 specular = specularStrength * spec * u_light_color;
		vec3 diffuse = vec3(max(dot(normal, lightDirection), 0.0)) * u_light_color;


		FragColor = vec4(ambient + diffuse + specular, 1.0) * texture(texture_diffuse, UV);
	}
}