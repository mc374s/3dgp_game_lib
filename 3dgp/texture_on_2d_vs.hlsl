#include "texture_on.hlsli"

VS_OUT main(float4 pos : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
	VS_OUT vout;

	vout.position = pos;
	vout.color = color;
	vout.texcoord = texcoord;

	return vout;
}