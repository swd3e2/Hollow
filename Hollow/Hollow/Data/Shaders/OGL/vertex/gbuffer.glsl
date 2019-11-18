#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIDs;
layout(location = 6) in vec4 weights;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

out VS_OUT
{
	vec4 position;
	vec3 normal;
	vec2 texCoord;
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
	vec3 color;
	bool selected;
	bool hasAnimation;
};

layout(std140, binding = 3) uniform WorldTransformations
{
	mat4 worldTransform;
};

layout(std140, binding = 6) uniform BoneTransformations
{
	mat4 boneInfo[200];
};

void main()
{
	vs_out.position = vec4(pos.x, pos.y, pos.z, 1.0f);
	vs_out.normal = normal;

	if (hasAnimation) {
		mat4 BoneTransform = boneInfo[boneIDs[0]] * weights[0];
		BoneTransform     += boneInfo[boneIDs[1]] * weights[1];
		BoneTransform     += boneInfo[boneIDs[2]] * weights[2];
		BoneTransform     += boneInfo[boneIDs[3]] * weights[3];

		vs_out.position = vs_out.position * BoneTransform;
		vs_out.normal = vs_out.normal * mat3(BoneTransform);
	} else {
		vs_out.position = vs_out.position * worldTransform;
	}

	vs_out.texCoord = texCoord;
	vs_out.position = vs_out.position * transform;

	vs_out.normal = normalize(vs_out.normal * mat3(worldTransform) * mat3(transform));
	vs_out.tangent = normalize(tangent * mat3(worldTransform) * mat3(transform));
	vs_out.bitangent = normalize(bitangent * mat3(worldTransform) * mat3(transform));

	gl_Position = vs_out.position * WVP;
}