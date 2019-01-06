struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 texCoord : NORMAL0;
    float3 normal : TEXCOORD0;
};

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}