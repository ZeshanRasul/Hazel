struct VSOut 
{
	float3 colour : Colour;
	float4 position : SV_Position;
};

cbuffer CBuf
{
	row_major matrix transform;
};

VSOut main(float2 pos : Position, float3 colour : Colour)
{
	VSOut vso;
	vso.position = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	vso.colour = colour;
	return vso;
}