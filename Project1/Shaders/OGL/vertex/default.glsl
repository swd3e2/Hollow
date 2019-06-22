#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in int[4] boneIDs;
layout(location = 6) in vec4 weights;

out VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 cubemapDirection;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
	mat4 WVP;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 cameraPosition;
	float pad2;
	bool selected;
	bool hasAnimation;
};

layout(std140, binding = 4) uniform MaterialData
{
	vec4 color;
	float metallicFactor;
	float roughnessFactor;
	float emmisiveFactor;
	float pad;
	bool hasDiffuseTexture;
	bool hasNormalMap;
	bool hasSpecularMap;
};

layout(std140, binding = 7) uniform Bones
{
	mat4 boneInfo[100];
};

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	vs_out.normal = normal;

	/*if (hasAnimation) {
		mat4 BoneTransform	= boneInfo[boneIDs[0]] * weights[0];
		BoneTransform	   += boneInfo[boneIDs[1]] * weights[1];
		BoneTransform	   += boneInfo[boneIDs[2]] * weights[2];
		BoneTransform	   += boneInfo[boneIDs[3]] * weights[3];

		gl_Position = gl_Position * BoneTransform;
	}*/
	vs_out.normal = normal * mat3(transform);

	gl_Position = gl_Position * transform;
	gl_Position = gl_Position * WVP;

	vs_out.texCoord = texCoord;
	vs_out.tangent = tangent;
	vs_out.bitangent = bitangent;

	vec3 temp = cameraPosition - vec3(gl_Position);
	vs_out.cubemapDirection = reflect(temp, normal);
}