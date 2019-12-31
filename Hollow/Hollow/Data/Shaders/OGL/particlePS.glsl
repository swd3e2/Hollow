#version 460

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;
layout(location = 3) out vec4 pbr;

in VS_OUT
{
	vec4 position;
	vec2 texCoord;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

layout(std140, binding = 3) uniform ParticleData
{
	int maxOffsets;
	int currentOffset;
	vec2 texCoord;
}; 

uniform sampler2D atlas; //0

void main() {
	vec4 color = texture(atlas, fs_in.texCoord);
	
	normal = vec4(0.0f, 1.0f, 0.0f, 0.0f) * 0.5 + 0.5;

	if (color.a < 0.25) {
		discard;
	}

	pbr = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	diffuse = color;
	position = fs_in.position;
}