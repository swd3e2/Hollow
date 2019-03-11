struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
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
    float power;
    float4 ambient;
};

cbuffer LightBuffer : register(b3)
{
    AmbientLight ambientLight;
    PointLight pointLight;
}

Texture2D ambient_map   : TEXUTRE : register(t0);
Texture2D normal_map    : TEXUTRE : register(t1);
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);

/*
    light.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	light.range = 100.0f;
	light.att = XMFLOAT3(0.0f, 0.2f, 0.0f);
	light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
*/
float4 addAmbientColor(float3 normal, float4 position)
{
    float4 color = saturate(dot(normal, ambientLight.direction) * ambientLight.ambient);

    float4 pointLightPosition = mul(float4(pointLight.position, 0.0f), World);
    float4 lightToPixel = pointLightPosition - position;
    
    float lenght = length(lightToPixel);
    lightToPixel = lightToPixel / lenght;

    if (lenght < pointLight.power)
    {
        float pointLightStrength = dot(lightToPixel, normalize(normal));
        if (pointLightStrength > 0.0f)
        {
            float attenuation = 1.0f / 0.0f + 0.125f * lenght + 0.021f * lenght * lenght;
            float4 lightColor = pointLight.ambient * pointLightStrength * pointLightStrength;
            color += lightColor;
        }
    }

    return color;
}

float4 PSMain(PixelShaderInput input) : SV_TARGET
{
    float4 color = ambient_map.Sample(SampleTypeClamp, input.texCoord);

    color += addAmbientColor(input.normal, input.worldPos);

    return color;
}