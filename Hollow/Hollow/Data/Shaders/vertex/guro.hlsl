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

cbuffer ConstantBuffer : register(b5)
{
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    float3 lightPosition;
}

struct PixelShaderOutput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
    float4 color : COLOR;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

PixelShaderOutput VSMain(VertexShaderInput input)
{
    PixelShaderOutput output;

    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, transform);
    output.worldPos = output.pos;

    output.pos = mul(output.pos, WVP);
    output.normal = mul(input.normal, transform);
    output.texCoord = input.texCoord;

    output.lightViewPosition = float4(input.pos, 1.0f);
    output.lightViewPosition = mul(output.lightViewPosition, transform);
    output.lightViewPosition = mul(output.lightViewPosition, World);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    output.lightPos = float3(0.0f, 0.0f, 0.0f);

    float3 lightToVertex = pointLight.position - output.pos;
    float d = length(lightToVertex);
    lightToVertex /= d;

    if (d < pointLight.range) {
        output.color = float4(max(dot(lightToVertex, normalize(output.normal)), 0.0f) * pointLight.color, 0.0f);
    } else {
        output.color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    return output;
}