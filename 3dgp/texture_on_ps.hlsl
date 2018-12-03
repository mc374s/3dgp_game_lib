#include "texture_on.hlsli"

Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{

	//return pin.color;
	
	//return diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord);

	float4 color = diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord);

	//float4 colorSum = { 0,0,0,0 };

	///*float mX = inputSize.x / 2.0 * inputPos.x;
	//float mY = inputSize.y / 2.0 * inputPos.y;*/
	//float mX = inputSize.x > 0 ? inputPos.x : 0;
	//float mY = inputSize.y > 0 ? inputPos.y : 0;
	//float mXX = inputSize.x * mX;
	//float mYY = inputSize.y * mY;
	//float tempX = 3;
	//float tempY = 3;
	//float2 adjustCoord[] = {
	//	{ -mX, -mY },
	//	{  mX, -mY },
	//	{ -mX,  mY },
	//	{  mX,  mY },

	//	{ -tempX*mX, -tempY*mY },
	//	{  tempX*mX, -tempY*mY },
	//	{ -tempX*mX,  tempY*mY },
	//	{  tempX*mX,  tempY*mY }
	//};

	//colorSum =
	//	  diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[0])
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[1])
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[2])
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[3])

	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[4])
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[5])
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[6])
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[7])

	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[0] + float2(-mXX, 0))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[1] + float2(-mXX, 0))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[2] + float2(-mXX, 0))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[3] + float2(-mXX, 0))

	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[4] + float2(0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[5] + float2(0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[6] + float2(0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[7] + float2(0, -mYY))

	//	/*+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[0] + float2(-0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[1] + float2(-0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[2] + float2(-0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[3] + float2(-0, -mYY))

	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[4] + float2(-0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[5] + float2(-0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[6] + float2(-0, -mYY))
	//	+ diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + adjustCoord[7] + float2(-0, -mYY))*/;

	//color = colorSum / 16.0;

	/*pin.color.r *= pin.color.a;
	pin.color.g *= pin.color.a;
	pin.color.b *= pin.color.a;
	color.r += pin.color.r;
	color.g += pin.color.g;
	color.b += pin.color.b;*/
	//color *= pin.color.a;
	/*if (color.r != 0)
	{
	color.r += pin.color.r;
	}
	if (color.g != 0)
	{
	color.g += pin.color.g;
	}
	if (color.b != 0)
	{
	color.b += pin.color.b;
	}
	color.a *= pin.color.a;*/
	// í èÌ
	color = color*pin.color;
	// îΩì]
	/*color.r = 1 - color.r;
	color.g = 1 - color.g;
	color.b = 1 - color.b;*/
	// íEêF
	/*float gray = (color.r + color.g + color.b) / 3.0;
	color.r = color.g = color.b = gray;*/
	// êFìhÇË
	/*color.r = pin.color.r;
	color.g = pin.color.g;
	color.b = pin.color.b;*/

	//if (color.g < 0.5 /*&& color.g > 0*/) {
	//	color.a = 0;
	//}
	/*static float dis = 0;
	dis += 0.2;

	if (distance(pin.texcoord, (0.5, 0.5)) < dis) {
	color.a = 0;
	}*/


	/*if (distance(pin.texcoord, inputPos) < inputSize.x)
	{
	color.a = 0;
	}*/

	return color;

}