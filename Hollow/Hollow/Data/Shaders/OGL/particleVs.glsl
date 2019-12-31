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
	vec2 texCoord;
} vs_out;
 
layout(std140, binding = 0) uniform Matrices
{
	mat4 WVP;
	vec3 cameraPosition;
	vec3 cameraRotation;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};
 
layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool selected;
	bool hasAnimation;
};
  
layout(std140, binding = 3) uniform ParticleData
{
	int maxOffsets;
	int currentOffset;
	vec2 pTexCoord;
};

void main()
{
	mat4 tempWVP = ViewMatrix;

	tempWVP[0][0] = 1.0f;
	tempWVP[0][1] = 0.0f;
	tempWVP[0][2] = 0.0f;

	tempWVP[1][0] = 0.0f;
	tempWVP[1][1] = 1.0f;
	tempWVP[1][2] = 0.0f;

	tempWVP[2][0] = 0.0f;
	tempWVP[2][1] = 0.0f;
	tempWVP[2][2] = 1.0f;

	tempWVP[3][3] = 1.0f;

	vs_out.position = vec4(pos, 1.0f) * transform;

	vs_out.texCoord = texCoord * pTexCoord;
	vs_out.texCoord.x += currentOffset % maxOffsets * pTexCoord.x;
	vs_out.texCoord.y += currentOffset / maxOffsets * pTexCoord.y;

	gl_Position = vs_out.position * tempWVP * ProjectionMatrix;
}