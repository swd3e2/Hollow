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
	vec4 shadowPos;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 cubemapDirection;
} vs_out;

layout(std140, binding = 0) uniform Matrices
{
	mat4 WVP;
	vec3 cameraPosition;
};

layout(std140, binding = 1) uniform ShadowMatrices
{
	mat4 ShadowWVP;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	float pad2;
	bool selected;
	bool hasAnimation;
};

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);

	vs_out.normal = normal;

	vs_out.normal = normalize(normal * mat3(transform));

	//gl_Position = gl_Position * transform;
	gl_Position = gl_Position * WVP;

	vs_out.texCoord = texCoord;
	vs_out.tangent = tangent;
	vs_out.bitangent = bitangent;

	vec3 temp = cameraPosition - vec3(gl_Position);
	vs_out.cubemapDirection = reflect(temp, normal);
	vs_out.shadowPos = vec4(pos.x, pos.y, pos.z, 1.0f) * WVP;
}