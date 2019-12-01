struct PixelShaderInput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D light : TEXTUTRE: register(t0);

SamplerState SamplerClamp : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	return light.Sample(SamplerClamp, input.texCoord);
}