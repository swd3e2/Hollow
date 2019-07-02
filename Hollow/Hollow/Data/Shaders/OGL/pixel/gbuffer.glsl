#version 460

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;

in VS_OUT
{
	vec4 position;
	vec3 normal;
	vec2 texCoord;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

uniform sampler2D ambient_map;

void main()
{
	vec4 color = texture(ambient_map, fs_in.texCoord);

	if (color.a < 0.55f) {
		discard;
	}

	diffuse = color;
	normal = vec4(fs_in.normal, 1.0f);
	position = fs_in.position;
}