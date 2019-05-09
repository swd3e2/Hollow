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

cbuffer ConstantBuffer : register(b5)
{
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    float3 lightPosition;
    float bias;    
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
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

PixelShaderOutput main(VertexShaderInput input)
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
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    float4 worldPosition = mul(input.pos, World);
    worldPosition = mul(worldPosition, transform);

    output.lightPos = lightPosition.xyz - worldPosition.xyz;
    output.lightPos = normalize(output.lightPos);

    output.tangent = input.tangent;
    output.bitangent = input.bitangent;

    return output;
}