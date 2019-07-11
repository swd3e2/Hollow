cbuffer ConstantBuffer : register(b0)
{
	matrix WVP;
}

cbuffer ConstantBuffer : register(b2)
{
	matrix transform;
}

struct SKYMAP_VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

struct VertexShaderInput
{
	float3 pos			: POSITION;
	float2 texCoord		: TEXCOORD;
};

SKYMAP_VS_OUTPUT main(VertexShaderInput input)
{
	SKYMAP_VS_OUTPUT output;

	output.pos = float4(input.pos, 1.0f);
	output.pos = mul(output.pos, WVP).xyww;
	output.texCoord = input.pos;

	return output;
}