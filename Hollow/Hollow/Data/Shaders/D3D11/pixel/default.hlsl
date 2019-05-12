struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL0;
};

Texture2D ambient_map   : TEXUTRE: register(t0);
Texture2D normal_map    : TEXUTRE: register(t1);
Texture2D specular_map  : TEXUTRE: register(t2);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float val = dot(input.normal, float3(-1.0f, 0.0f, 0.0f));
	return ambient_map.Sample(SampleTypeWrap, input.texCoord) + val * float4(0.5f, 0.5f, 0.5f,0.0f);
}