#version 330 core

in vec2 UV;
in vec3 normal;
in vec4 vertexColor;

uniform vec3 u_color;
uniform sampler2D texture_diffuse;

out vec4 FragColor;

void main()
{
	FragColor = vertexColor * texture(texture_diffuse, UV);
	//FragColor = vertexColor;
}