#version 460

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;

in VS_OUT
{
	vec4 position;
	vec3 normal;
	vec2 texCoord;
	vec3 tangent;
	vec3 bitangent;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

layout(std140, binding = 3) uniform WorldTransformations
{
	mat4 worldTransform;
};

layout(std140, binding = 4) uniform MaterialData
{
	vec4 base_color;
	float metallicFactor;
	float roughnessFactor;
	float emmisiveFactor;
	float pad;
	bool hasDiffuseTexture;
	bool hasNormalTexture;
	bool hasSpecularMap;
};

uniform sampler2D diffuseTexture; //0
uniform sampler2D normalTexture;  //1

void main()
{
	vec4 color;
	if (hasDiffuseTexture) {
		color = texture(diffuseTexture, fs_in.texCoord);
	} else {
		color = base_color;
	}
	if (hasNormalTexture) {
		mat3 tanToView = transpose(mat3(
			fs_in.tangent, 
			fs_in.bitangent, 
			fs_in.normal
		));

		vec3 temp = normalize(texture(normalTexture, fs_in.texCoord).xyz * 2.0 - 1.0);
		
		temp = normalize(temp * tanToView);
		normal = vec4(temp, 0.0f) * 0.5 + 0.5;
		//normal = vec4(fs_in.normal, 1.0f) * 0.5 + 0.5;
		//normal = normalize(vec4(fs_in.tangent, 0.0f) * 2.0 - 1.0);
	/*	normal.x = -normal.x;
		normal.z = -normal.z;*/
	} else {
		
		normal = vec4(fs_in.normal, 1.0f) * 0.5 + 0.5;
	}

	if (color.a < 0.25) {
		discard;
	}

	diffuse = color;
	position = fs_in.position;
}