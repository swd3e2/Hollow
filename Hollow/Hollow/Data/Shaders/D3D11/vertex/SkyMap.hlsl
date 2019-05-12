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
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 bitangent	: BITANGENT;
	int4 boneId			: BONEID;
	float4 weight		: WEIGHT;
};

SKYMAP_VS_OUTPUT main(VertexShaderInput input)
{
	SKYMAP_VS_OUTPUT output;
	
	output.pos = float4(input.pos, 1.0f);
	output.pos = mul(output.pos, transform);
	output.pos = mul(output.pos, WVP).xyww;
	output.texCoord = input.pos;

	return output;
}