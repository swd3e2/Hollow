#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

uniform sampler2D ambient_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;

void main()
{
	FragColor = texture(normal_map, vec2(fs_in.texCoord.x, 1.0f-fs_in.texCoord.y));
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}