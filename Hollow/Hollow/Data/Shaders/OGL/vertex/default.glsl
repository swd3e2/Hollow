#version 460

struct VSInput
{
	vec3 pos;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec4 boneIDs;
	vec4 weights;
};

interface VSOutput
{
	vec2 texCoord;
	vec3 normal;
};

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(in VSInput VSin : 0, out VSOutput VSout)
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	gl_Position = gl_Position * viewMatrix * projectionMatrix;

	VSout.TexCoord = VSin.TexCoord;
	VSout.normal = vec4(VSin.normal, 0.0);
}