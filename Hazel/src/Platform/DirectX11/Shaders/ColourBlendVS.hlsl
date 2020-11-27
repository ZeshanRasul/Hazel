cbuffer CBuf
{
	matrix transform;
};

struct VSOut
{
	float4 colour : COLOUR;
	float4 position : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float4 colour : COLOUR) 
{
	VSOut vso;
	vso.position = mul(float4(pos, 1.0f), transform);
	vso.colour = colour;
	return vso;
}