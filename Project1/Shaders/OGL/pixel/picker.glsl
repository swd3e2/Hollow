#version 460

out vec4 FragColor;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

void main()
{
	FragColor = vec4(color, 1.0f);
}