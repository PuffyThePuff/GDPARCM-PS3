#version 330 core

in vec2 UV;

uniform vec3 u_color;
uniform sampler2D texture_diffuse;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture_diffuse, UV);
	//FragColor = vec3(u_color);
}