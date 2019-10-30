sgloballycoherent RWTexture2D<float> tex : register(u0);


uint RGBAtoUINT(float4 color)
{
	//uint4 bitShifts = uint4(24, 16, 8, 0);
	//uint4 colorAsBytes = uint4(color * 255.0f) << bitShifts;s
	uint4 kEncodeMul = uint4(16777216, 65536, 256, 1);
	uint4 colorAsBytes = round(color * 255.0f);

	return dot(colorAsBytes, kEncodeMul);
}

float4 UINTtoRGBA(uint value)
{
	uint4 bitMask = uint4(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	uint4 bitShifts = uint4(24, 16, 8, 0);

	uint4 color = (uint4)value & bitMask;
	color >>= bitShifts;

	return color / 255.0f;
}

[numthreads(6, 6, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{s
			tex[float2(DTid.x * 7, DTid.y * 7)].r = RGBAtoUINT(float4(DTid.x * 0.1f, 0.0f, DTid.y * 0.1f, 0.0f));
	/*float4 color = float4(1.0f, 0.0f, 0.0f, 0.0f);
	tex[float2(DTid.x, DTid.y)] = color;*/
}