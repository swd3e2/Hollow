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

layout(std140, binding = 4) uniform MaterialData
{
	vec4 base_color;
	float metallicFactor;
	float roughnessFactor;
	float emmisiveFactor;
	float pad;
	bool hasDiffuseTexture;
	bool hasNormalMap;
	bool hasSpecularMap;
};

uniform sampler2D tex0; //0

void main()
{
	vec4 color;
	if (hasDiffuseTexture) {
		color = texture(tex0, fs_in.texCoord);
	}
	else {
		color = base_color;
	}

	if (color.a < 0.25) {
		discard;
	}

	diffuse = color;
	normal = vec4(fs_in.normal, 1.0f);
	position = fs_in.position;
}