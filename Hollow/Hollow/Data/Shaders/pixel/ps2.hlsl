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

struct AmbientLight
{
    float3 direction;
    float pad;
    float4 ambient;
};

struct PointLight
{
    float3 position;
    float power;
    float4 ambient;
};

cbuffer LightBuffer : register(b3)
{
    AmbientLight ambientLight;
    PointLight pointLight;
}

Texture2D ambient_map : TEXUTRE : register(t0);
Texture2D normal_map : TEXUTRE : register(t1);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 addAmbientColor(float3 normal)
{
    return saturate(dot(normal, ambientLight.direction) * ambientLight.ambient);
}

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color = normal_map.Sample(SampleTypeClamp, input.texCoord);
    color = color * 2 - 1;

    return color;
}