#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in int[4] boneIDs;
layout(location = 6) in vec4 weights;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

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
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);

	if (hasAnimation) {
		mat4 BoneTransform = boneInfo[boneIDs[0]] * weights[0];
		BoneTransform     += boneInfo[boneIDs[1]] * weights[1];
		BoneTransform     += boneInfo[boneIDs[2]] * weights[2];
		BoneTransform     += boneInfo[boneIDs[3]] * weights[3]; 

		gl_Position = gl_Position * BoneTransform;
	} else {
		gl_Position = gl_Position * worldTransform;
	}

	gl_Position = gl_Position * transform;
	gl_Position = gl_Position * WVP;
}