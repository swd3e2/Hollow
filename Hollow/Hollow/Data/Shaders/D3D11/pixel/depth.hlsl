struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 spos : SHADOW_POSITION;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float depth = input.spos.z / input.spos.w;
    return float4(depth, depth, depth, 1.0f);
}