float4x4 view_proj_matrix;
float4x4 texture_matrix0;

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float3 texCoord : TEXCOORD0;
};

VS_OUTPUT main (float4 vPosition : POSITION)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

	//convert position to clip space
	Out.Pos = mul (view_proj_matrix, vPosition)
}