#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal;

out vec2 UV;
out vec3 normal;
out vec3 vertexColor;
out vec3 fragPosition;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
	UV = a_UV;
	normal = normalize(mat3(u_normal) * a_normal);
}