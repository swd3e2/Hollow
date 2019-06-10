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
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
	mat4 WVP;
	vec3 cameraPosition;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	bool hasAnimation;
};

layout(std140, binding = 7) uniform Bones
{
	mat4 boneInfo[100];
};

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	vs_out.normal = normal;

	if (hasAnimation) {
		mat4 BoneTransform	= boneInfo[boneIDs[0]] * weights[0];
		BoneTransform	   += boneInfo[boneIDs[1]] * weights[1];
		BoneTransform	   += boneInfo[boneIDs[2]] * weights[2];
		BoneTransform	   += boneInfo[boneIDs[3]] * weights[3];

		gl_Position = gl_Position * BoneTransform;
	}
	vs_out.normal = normal;

	gl_Position = gl_Position * transform;
	gl_Position = gl_Position * WVP;

	vs_out.texCoord = texCoord;
	vs_out.tangent = tangent;
	vs_out.bitangent = bitangent;
}