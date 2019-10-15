#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
} fs_in;

uniform float lightTypeAmbient = 0;
uniform float lightTypeDiffuse = 1;
uniform float lightTypePoint = 2;
uniform float lightTypeSpot = 3;

struct Light
{
	vec3 position;		// 12
	float constant;		// 16
	vec3 direction;		// 28
	float linearAttenuation;		// 32
	vec3 color;			// 44
	float quadratic;	// 48
	float cutoff;		// 52
	float distance;		// 56
	int type;			// 60
	int pad;			// 64
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 WVP;
	vec3 cameraPosition;
	vec3 cameraViewDirection;
};

layout(std140, binding = 1) uniform ShadowMatrices
{
	mat4 ShadowWVP;
	vec2 texelSize;
	float bias;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

layout(std140, binding = 5) uniform LightData
{
	Light[100] lights;
	int numLights;
};

uniform sampler2D tex0; //0
uniform sampler2D tex1; //1
uniform sampler2D tex2; //2
uniform sampler2D tex3; //3

void main()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}