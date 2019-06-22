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
    float pad1;
};

cbuffer LightBuffer : register(b3)
{
    PointLight pointLight[1];
}

cbuffer MateriaBuffer : register(b4)
{
    Material material;
}

cbuffer ConstantBuffer : register(b5)
{
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    float3 lightPosition;
    float bias;    
}

cbuffer ConstantBuffer : register(b6)
{
    PointLight pointLights[10];
    float pointLightCount;
}

Texture2D ambient_map   : TEXUTRE : register(t0);
Texture2D normal_map    : TEXUTRE : register(t1);
Texture2D specular_map  : TEXUTRE : register(t2);
Texture2D shadow_map	: TEXUTRE : register(t3);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 addPointLight(PointLight light, float3 normal, float4 position, float2 texCoords)
{
    float4 color = float4(0.f, 0.f, 0.f, 0.f);

    // point light
    float4 pointLightPosition = float4(light.position, 0.0f);
    float3 lightToPixel = pointLightPosition - position;
  
    float lenght = length(lightToPixel);
    // normalize vector
    lightToPixel /= lenght;
  
    if (lenght > light.range)
    {
        return color;
    }

    float howMuchLight = dot(lightToPixel, normal);

    color += float4(light.color, 1.0f) *
    saturate(1 / light.attenuation[0] +
    (light.attenuation[1] * lenght) +
    (light.attenuation[2] * (lenght * lenght)));
    
    float4 specularIntensity = float4(1.0f, 1.0f, 1.0f, 0.0f);
    if (material.hasSpecularMap)
    {
        specularIntensity = specular_map.Sample(SampleTypeClamp, texCoords);
    }
    
    // specular light
    float3 V = normalize(cameraPosition - position);
    float3 R = reflect(-lightToPixel, normal);

    color = color + saturate(pow(max(dot(R, V), 0.0f), material.Ns) * specularIntensity);

    return color;
}

float3 calculateNormals(float2 texCoord, float3 normal, float3 tangent, float3 bitangent)
{
    float4 normalMap = normal_map.Sample(SampleTypeWrap, texCoord);
    normalMap = (2.0f * normalMap) - 1.0f;
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    float3x3 texSpace = float3x3(tangent, bitangent, normal);

    return mul(normalMap, texSpace);
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 color;
    if (material.hasDiffuseTexture) {
        color = ambient_map.Sample(SampleTypeWrap, input.texCoord);
        color.xyz *= material.Kd;
    } else {
        color = float4(0.5f, 0.5f, 0.5f, 1.0f);
    }

    if (material.hasNormalMap) {
        input.normal = calculateNormals(input.texCoord, input.normal, input.tangent, input.bitangent);
    }

    input.normal = normalize(input.normal);

    // Set the bias value for fixing the floating point precision issues.
    float2 projectTexCoord;

    // Calculate the projected texture coordinates.
    projectTexCoord.x = (input.lightViewPosition.x) / input.lightViewPosition.w * 0.5f + 0.5f;
    projectTexCoord.y = (-input.lightViewPosition.y) / input.lightViewPosition.w * 0.5f + 0.5f;

    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        float4 shadowMap = shadow_map.Sample(SampleTypeClamp, float2(projectTexCoord.x, projectTexCoord.y));
        color -= shadowMap.r < input.lightViewPosition.z / input.lightViewPosition.w ? 0.3f : 0.0f;
    }
           
    for (int i = 0; i < 2; i++)
    {
        color += addPointLight(pointLights[i], input.normal, input.worldPos, input.texCoord);
    }

    return color;
}