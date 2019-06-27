#version 460

in VS_OUT
{
	vec4 pos;
} pin;

out vec4 FragColor;

void main()
{
	float depthValue = pin.pos.z / pin.pos.w;
	FragColor = vec4(depthValue, depthValue, depthValue, 1.0f);
}