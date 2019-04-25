#include "common.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

float4 diffuse(VSOUT psIn) : SV_TARGET
{	
    return psIn.diffuse;
}

float4 diffuse_tex(VSOUT_Tex psIn) : SV_TARGET
{
    return diffuseMap.Sample(diffuseMapSamplerState, psIn.texcoord) * psIn.diffuse;
}