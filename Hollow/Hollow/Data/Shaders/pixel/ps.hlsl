struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
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
    float3 attenuation;
    float pad;
};

cbuffer LightBuffer : register(b3)
{
    PointLight pointLight;
}

cbuffer MateriaBuffer : register(b4)
{
    Material material;
}

Texture2D ambient_map   : TEXUTRE : register(t0);
Texture2D normal_map    : TEXUTRE : register(t1);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 addAmbientColor(float3 normal, float4 position)
{
    //float4 color = saturate(dot(normal, ambientLight.direction) * ambientLight.ambient);
    float4 color = float4(0.f, 0.f, 0.f, 0.f);
    // point light
    float4 pointLightPosition = float4(pointLight.position, 0.0f);
    float4 lightToPixel = pointLightPosition - position;
  
    float lenght = length(lightToPixel);
    // normalize vector
    lightToPixel /= lenght;
  
    if (lenght < pointLight.range)
    {
        float howMuchLight = dot(lightToPixel, normal);
        if (howMuchLight > 0.0f)
        {
            float4 pointColor = howMuchLight * float4(pointLight.color, 0.0f);
            pointColor /= pointLight.attenuation[0] + (pointLight.attenuation[1] * lenght) + (pointLight.attenuation[2] * lenght * lenght);
            color *= pointColor;
        }
    }

    // specular light
    float3 R = reflect(-lightToPixel, normal);
    float3 V = normalize(cameraPosition - position);
    color += pow(max(dot(R, V), 0.0f), 3.0f); // change to Ns later

    return color;
}

float3 calculateNormals(float2 texCoord, float3 normal, float3 tangent, float3 bitangent)
{
    float4 normalMap = normal_map.Sample(SampleTypeClamp, texCoord);
    normalMap = (2.0f * normalMap) - 1.0f;
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    float3x3 texSpace = float3x3(tangent, bitangent, normal);

    return normalize(mul(normalMap, texSpace));
}

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color;
    if (material.hasDiffuseTexture) {
        color = ambient_map.Sample(SampleTypeClamp, input.texCoord);
    } else {
        color = float4(0.5f, 0.5f, 0.5f, 1.0f);
    }

    color.xyz -= 0.3f;
    if (material.hasNormalMap){
        input.normal = calculateNormals(input.texCoord, input.normal, input.tangent, input.bitangent);
    }

    color += addAmbientColor(input.normal, input.worldPos);
    return color;
}