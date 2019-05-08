struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
    float4 color : COLOR;
};

struct Material
{
    float3 Ka;
    float Ns;
    float3 Kd;
    float pad;
    float3 Ks;
    float pad1;
    float3 Ke;
    float pad2;
    bool hasDiffuseTexture;
    bool hasNormalMap;
    bool hasSpecularMap;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix WVP;
}

cbuffer ConstantBuffer : register(b1)
{
    matrix World;
    matrix View;
    matrix Projection;
    float3 cameraPosition;
}

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
    float range;
    float3 color;
    float pad;
    float3 attenuation;
};

cbuffer LightBuffer : register(b3)
{
    PointLight pointLight;
}

cbuffer MateriaBuffer : register(b4)
{
    Material material;
}

Texture2D ambient_map   : TEXTUTRE : register(t0);
Texture2D normal_map    : TEXTUTRE : register(t1);
Texture2D specular_map  : TEXTUTRE : register(t2);
Texture2D shadow_map	: TETXUTRE : register(t3);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color;
    if (material.hasDiffuseTexture) {
        color = ambient_map.Sample(SampleTypeClamp, input.texCoord);
        color.xyz *= material.Kd;
    } else {
        color = float4(0.5f, 0.5f, 0.5f, 1.0f);
    }

    // Set the bias value for fixing the floating point precision issues.
    color += input.color;
    return color;
}