#version 460

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 position;

in VS_OUT
{
	vec4 position;
	vec3 normal;
	vec2 texCoord;
	vec3 inColor;
} fs_in;

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

uniform sampler2D tex0;

void main()
{
	vec4 diffuseColor = texture(tex0, fs_in.texCoord);
	if (diffuseColor.a < 0.55) {
		discard;
	}

	diffuse = clamp(diffuseColor * vec4(fs_in.inColor, 1.0f) * 2.0f, 0.0f, 1.0f);
	normal = vec4(fs_in.normal, 1.0f);
	position = fs_in.position;
}