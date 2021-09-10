#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal;
uniform vec3 u_light_position;
uniform vec3 u_camera_position;
uniform vec3 u_ambient_color;
uniform vec3 u_light_color;

out vec2 UV;
out vec3 normal;
out vec3 vertexColor;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);

	UV = a_UV;
	// normal = a_normal;
	normal = normalize(mat3(u_normal) * a_normal);

	vec3 vertexPosition = vec3(u_model * vec4(a_pos, 1.0));
	vec3 lightVector = normalize(u_light_position - vertexPosition);

	float specularStrength = 1.0f;
	vec3 viewDirection = normalize(u_camera_position - vertexPosition);
	vec3 reflectDirection = reflect(-lightVector, normal);
	float spec = pow(max(dot(reflectDirection, viewDirection), 0.0f), 16);

	vec3 specular = specularStrength * spec * u_light_color;
	vec3 diffuse = vec3(max(dot(a_normal, lightVector), 0.0)) * u_light_color;
	vec3 ambient = u_ambient_color * u_light_color;

	vertexColor = ambient + diffuse + specular;
}