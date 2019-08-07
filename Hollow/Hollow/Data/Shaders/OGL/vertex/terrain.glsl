#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 inColor;

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
	vec3 outColor;
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
	vs_out.outColor = inColor;
	gl_Position = vs_out.position * WVP;
}