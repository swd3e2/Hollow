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

Texture2D normal_map    : TEXTUTRE: register(t5);
SamplerState SamplerTypeWrap : register(s0);

float4 main(DomainOut input) : SV_TARGET
{
	float displacment = normal_map.Sample(SamplerTypeWrap, input.texCoord);

	return displacment;
}