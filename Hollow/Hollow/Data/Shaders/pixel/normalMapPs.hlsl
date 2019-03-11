struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return float4(input.normal.x, input.normal.y, input.normal.z, 0.0f);
}