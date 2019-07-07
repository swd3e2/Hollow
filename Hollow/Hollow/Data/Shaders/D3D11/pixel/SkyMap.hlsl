struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
	float4 pos		: SV_POSITION;
	float3 texCoord : TEXCOORD0;
};

TextureCube SkyMap	: TEXTUTRE: register(t4);
Texture2D depth		: TEXTUTRE: register(t5);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 main(SKYMAP_VS_OUTPUT input) : SV_Target
{
	float depth1 = depth.Sample(SampleTypeClamp, float2(input.pos.x / 1920.0f, input.pos.y / 1080.0f)).r;

	if (depth1 == 1.0f) {
		return SkyMap.Sample(SampleTypeWrap, input.texCoord);
	}
	discard;
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}