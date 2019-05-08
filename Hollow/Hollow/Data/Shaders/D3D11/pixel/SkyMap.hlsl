struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
	float4 pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

TextureCube SkyMap : TEXTUTRE: register(t0);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 PSMain(SKYMAP_VS_OUTPUT input) : SV_Target
{
	float4 color = SkyMap.Sample(SampleTypeWrap, input.texCoord);
	return color;
}