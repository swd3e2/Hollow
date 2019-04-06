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
Texture2D specular_map  : TEXUTRE : register(t2);
Texture2D shadow_map : TEXUTRE : register(t3);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

float4 addAmbientColor(float3 normal, float4 position, float2 texCoords)
{
    float4 color = float4(0.f, 0.f, 0.f, 0.f);

    // point light
    float4 pointLightPosition = float4(pointLight.position, 1.0f);
    float3 lightToPixel = pointLightPosition - position;
  
    float lenght = length(lightToPixel);
    // normalize vector
    lightToPixel /= lenght;
  
    if (lenght > pointLight.range)
    {
        return color;
    }

    float howMuchLight = dot(lightToPixel, normal);

    color += float4(pointLight.color, 1.0f) * 
    (1 / pointLight.attenuation[0] + 
    (pointLight.attenuation[1] * lenght) + 
    (pointLight.attenuation[2] * (lenght * lenght)));
    
    float4 specularIntensity = float4(1.0f, 1.0f, 1.0f, 0.0f);
    if (material.hasSpecularMap)
    {
        specularIntensity = specular_map.Sample(SampleTypeClamp, texCoords);
    }
    
    // specular light
    float3 V = normalize(cameraPosition - position);
    color = saturate(color + pow(min(dot(normalize(V + lightToPixel), normal), 0.0f), material.Ns) * specularIntensity);

    return color;
}

float3 calculateNormals(float2 texCoord, float3 normal, float3 tangent, float3 bitangent)
{
    float4 normalMap = normal_map.Sample(SampleTypeClamp, texCoord);
    normalMap = (2.0f * normalMap) - 1.0f;
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    float3x3 texSpace = float3x3(tangent, bitangent, normal);

    return mul(normalMap, texSpace);
}

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color;
    if (material.hasDiffuseTexture) {
        color = ambient_map.Sample(SampleTypeClamp, input.texCoord);
        color.xyz *= material.Kd;
    } else {
        color = float4(0.5f, 0.5f, 0.5f, 1.0f);
    }

    if (material.hasNormalMap) {
        input.normal = calculateNormals(input.texCoord, input.normal, input.tangent, input.bitangent);
    }

    input.normal = normalize(input.normal);

    // Set the bias value for fixing the floating point precision issues.
    float bias = 0.0035f;

    float2 projectTexCoord;

    // Calculate the projected texture coordinates.
    projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w * 0.5f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w * 0.5f + 0.5f;
    projectTexCoord = saturate(projectTexCoord);

    float4 shadowMap = shadow_map.Sample(SampleTypeClamp, projectTexCoord);

    if (shadowMap.r < (input.lightViewPosition.z / input.lightViewPosition.w - bias))
    {
        color -= 0.5f;
    }

    color += addAmbientColor(input.normal, input.worldPos, input.texCoord);

    return color;
}