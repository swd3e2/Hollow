RWTexture2D<float4> tex : register(u0);

[numthreads(12, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	tex[float2(DTid.x, DTid.y)] = float4(1.0f, 1.0f, 1.0f, 0.0f);
}