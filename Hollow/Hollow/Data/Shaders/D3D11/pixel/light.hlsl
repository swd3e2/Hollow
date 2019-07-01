struct PixelShaderOutput
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D diffuseBuffer : TEXTUTRE: register(t0);
Texture2D normalBuffer : TEXTUTRE: register(t1);
Texture2D positionBuffer : TEXTUTRE: register(t2);

SamplerState SampleTypeClamp : register(s0);

float4 main(PixelShaderOutput input) : SV_TARGET
{
	return diffuseBuffer.Sample(SampleTypeClamp, input.texCoord);
}