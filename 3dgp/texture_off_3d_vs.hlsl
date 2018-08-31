#include "texture_off.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL)
{
	VS_OUT vout;
	vout.position = mul(position, worldViewProjection);

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(lightDirection);

	//vout.color = materialColor * max(0, -dot(L, N));
	//vout.color.a = materialColor.a;

	vout.color = materialColor * color * max(0, -dot(L, N));
	vout.color.a = color.a;

	return vout;
}