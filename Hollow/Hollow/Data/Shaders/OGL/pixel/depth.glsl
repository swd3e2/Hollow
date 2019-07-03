#version 460

in VS_OUT
{
	vec4 pos;
	vec3 normal;
} pin;

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.3f, 0.3f, 0.3f,1.0f) + clamp(dot(pin.normal, vec3(0.0f, 1.0f, 0.0f)), 0.0f, 1.0f) * vec4(0.5f, 0.5f, 0.5f, 1.0f);
}