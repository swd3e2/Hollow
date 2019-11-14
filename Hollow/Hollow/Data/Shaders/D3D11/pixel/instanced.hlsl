struct PixelShaderInput
{
    float4 hPos : HPOS;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct PixelOutDeffered
{
    float4 diffuse : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 position : SV_TARGET2;
};
cbuffer ConstantBuffer : register(b2)
{
    matrix transform;
    float3 color;
    bool hasAnimation;
};

cbuffer ConstantBuffer : register(b4)
{
    float4 base_color;
    float metallicFactor;
    float roughnessFactor;
    float emmisiveFactor;
    float pad;
    bool hasDiffuseTexture;
    bool hasNormalMap;
    bool hasSpecularMap;
};

Texture2D ambient_map : TEXTUTRE : register(t0);

SamplerState Sampler : register(s1);

PixelOutDeffered main(PixelShaderInput input) : SV_TARGET
{
    PixelOutDeffered output;
    if (hasDiffuseTexture)
    {
        output.diffuse = ambient_map.Sample(Sampler, input.texCoord);
    }
    else
    {
        output.diffuse = base_color;
    }

    output.normal = float4(input.normal, 1.0f);
    output.position = input.hPos;

    return output;
}