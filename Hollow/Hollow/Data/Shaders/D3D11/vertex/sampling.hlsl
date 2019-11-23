struct PixelShaderOutput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderInput
{
	float3 pos		: POSITION;
	float2 texCoord	: TEXCOORD;
};

PixelShaderOutput main(VertexShaderInput input)
{
	PixelShaderOutput output;

	output.pos = float4(input.pos, 1.0f);
	output.texCoord = input.texCoord;

	return output;
}