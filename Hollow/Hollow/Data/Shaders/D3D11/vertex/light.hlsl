struct PixelShaderOutput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderInput
{
	float3 pos				: POSITION;
	float2 texCoord			: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float3 bitangent		: BITANGENT;
	int4   boneId			: BONEID;
	float4 weight			: WEIGHT;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;

	output.pos = float4(input.pos, 1.0f);
	output.texCoord = input.texCoord;

	return output;
}