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
	vec4 position;
	vec3 normal;
	vec2 texCoord;
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
	bool hasAnimation;
};

void main()
{
	vs_out.normal = normalize(normal * mat3(transform));
	vs_out.texCoord = texCoord;
	vs_out.position = vec4(pos.x, pos.y, pos.z, 1.0f) * transform;
	gl_Position = vs_out.position * WVP;
}