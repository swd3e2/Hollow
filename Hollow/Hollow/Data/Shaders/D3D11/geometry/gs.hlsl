struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VertexShaderOutput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

[maxvertexcount(3)]
void main(triangle VertexShaderOutput input[3], inout TriangleStream<GSOutput> output)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i].pos;
        element.texCoord = input[i].texCoord;
        element.normal = input[i].normal;
        element.tangent = input[i].tangent;
        element.bitangent = input[i].bitangent;

		output.Append(element);
	}
}