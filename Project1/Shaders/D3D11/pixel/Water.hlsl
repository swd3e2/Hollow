struct DomainOut
{
	float4 pos     	: SV_POSITION;
	float2 texCoord : TEXCOORD;
};

cbuffer ConstantBuffer : register(b1)
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 cameraPosition;
	float offset;
}

float4 UINTtoRGBA(uint value)
{
	uint4 bitMask = uint4(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	uint4 bitShifts = uint4(24, 16, 8, 0);

	uint4 color = (uint4)value & bitMask;
	color >>= bitShifts;

	return color / 255.0f;
}

Texture2D normal_map    : TEXTUTRE: register(t5);
SamplerState SamplerTypeWrap : register(s0);

float4 main(DomainOut input) : SV_TARGET
{
	uint displacment = normal_map.Sample(SamplerTypeWrap, input.texCoord).r;

	return UINTtoRGBA(displacment);
}