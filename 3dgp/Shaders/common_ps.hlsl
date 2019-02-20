#include "common.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

float4 GetDiffuseColor(float2 texcoord)
{
    return diffuseMap.Sample(diffuseMapSamplerState, texcoord);
}

float4 main(VertexShaderOutput vsInput) : SV_TARGET
{
    float4 color = 0;
    
    if (vsInput.psFlag & PS_TEXTURE_ON)
    {
        //color = GetDiffuseColor(vsInput.texcoord);
        color = diffuseMap.Sample(diffuseMapSamplerState, vsInput.texcoord) * vsInput.color;
    }
    else if (vsInput.psFlag & PS_TEXTURE_OFF)
    {
        color = vsInput.color;
    }

	
    return color;
}