struct GSOutput
{
	float4 pos 	 : SV_POSITION;
	float4 color : COLOR;
};

struct PixelShaderOutput
{
    float4 position : SV_POSITION;
    float4 color 	: COLOR;
};

[maxvertexcount(2)]
void main(
	point PixelShaderOutput input[1],  
	inout LineStream<GSOutput> output
) {
	{
		GSOutput element;
		element.pos = input[0].position + float4(0.1f, 0.0f, 0.0f, 0.0f);
		element.color = input[0].color;
		output.Append(element);
	}
	{
		GSOutput element;
		element.pos = input[0].position + float4(-0.1f, 0.0f, 0.0f, 0.0f);
		element.color = input[0].color;
		output.Append(element);
	}
}