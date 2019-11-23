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
	vec4 color;
} vs_out;

void main()
{
	vs_out.color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);

	//gl_Position = gl_Position * WVP;
}