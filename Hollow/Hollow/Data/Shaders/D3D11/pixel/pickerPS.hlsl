struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
    float id;
}

Texture2D objTexture : TEXUTRE : register(t0);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return float4(id % 255 * 0.01f, 0.0f, 0.f, 1.0f);
}