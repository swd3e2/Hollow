struct PixelShaderOutput
{
    float4 position : SV_POSITION;
    float4 color 	: COLOR;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
};

PixelShaderOutput main(VertexShaderInput input, uint instanceID : SV_InstanceID)
{
    PixelShaderOutput output;

    output.position = float4(input.pos, 1.0f);
    output.color = float4(1.0f, 0.0f, 1.0f, 1.0f);

    return output;
}