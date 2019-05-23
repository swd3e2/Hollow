#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in vec4 boneIDs;
layout(location = 6) in vec4 weights;

out vec2 outtexCoord;
out vec3 outnormal;
out vec3 outtangent;
out vec3 outbitangent;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	gl_Position = gl_Position * viewMatrix * projectionMatrix;
	outtexCoord = texCoord;
	outnormal = normal;
	outtangent = tangent;
	outbitangent = bitangent;
}