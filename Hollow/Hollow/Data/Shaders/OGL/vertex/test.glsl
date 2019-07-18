#version 460

layout(location = 0) in vec3 pos;

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

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
}