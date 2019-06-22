#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 cubemapDirection;
} fs_in;

layout(std140, binding = 4) uniform MaterialData
{
	vec4 color;
	float metallicFactor;
	float roughnessFactor;
	float emmisiveFactor;
	float pad;
	bool hasDiffuseTexture;
	bool hasNormalMap;
	bool hasSpecularMap;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 cameraPosition;
	float pad2;
	bool selected;
	bool hasAnimation;
};

uniform sampler2D ambient_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D shadow_map;
uniform samplerCube enivironmentMap;

void main()
{
	if (selected) {
		FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	} else {
		FragColor = texture(ambient_map, fs_in.texCoord) + (dot(fs_in.normal, vec3(1.0f, 0.0f, 0.0f)) * vec4(0.5f, 0.5f, 0.5f, 1.0f));
		//FragColor = roughnessFactor * texture(enivironmentMap, fs_in.cubemapDirection);
	}
	//FragColor += dot(fs_in.normal, vec3(0.0f, 1.0f, -0.5f)) * vec4(0.5f, 0.5f, 0.5f, 1.0f);
}