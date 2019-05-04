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
}

cbuffer LightBuffer : register(b3)
{
    float3 direction;
    float pad;
    float4 ambient;
}

Texture2D ambient_map : TEXUTRE : register(t0);
Texture2D normal_map : TEXUTRE : register(t1);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 addAmbientColor(float3 normal)
{
    return saturate(dot(normal, direction) * ambient);
}

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    return float4(input.tangent, 1.0f);
}