#include "common.hlsli"

#define BONE_INFLUENCE_NUM_MAX (4)

struct AppData
{
    float4 position : SV_Position;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
    uint blendIndices[4] : BLENDINDICES;
    float blendWeight[4] : BLENDWEIGHT;
};

cbuffer PerApplication : register(b0)
{
    row_major matrix projection;
};

cbuffer PerFrame : register(b1)
{
    row_major matrix view;
};

cbuffer PerObject : register(b2)
{
    row_major matrix world;
    row_major matrix worldViewProjection;
    float4 materialColor;
    float4 lightDirection;

    uint vsFlag;
    uint psFlag;
    float padding[2];

}; // (16*4 + 16*4 + 4 + 4 + 8) = 9 * 16 bytes

#define MAX_BONES (32)
cbuffer PerObjectBoneBlend : register(b3)
{
    row_major matrix boneTransforms[MAX_BONES];
};



VertexShaderOutput main(AppData appData)
{
    VertexShaderOutput vsOutput;
	
    vsOutput.position = float4(0, 0, 0, 0);
    vsOutput.normal = float4(0, 0, 0, 0);
    vsOutput.tangent = float4(0, 0, 0, 0);
    vsOutput.color = float4(0, 0, 0, 0);
    vsOutput.texcoord = float2(0, 0);
	
    vsOutput.materialColor = float4(0, 0, 0, 0);
    vsOutput.lightDirection = float4(0, 0, 0, 0);
    vsOutput.psFlag = psFlag;

    if (psFlag & PS_TEXTURE_ON)
    {
        vsOutput.texcoord = appData.texcoord;
    }

    if (vsFlag & VS_PROJECTION_OFF)
    {
        vsOutput.position = appData.position;
        vsOutput.color = appData.color;
    }
    else if (vsFlag & VS_PROJECTION_ON)
    {   
        if (vsFlag & VS_SKINNED_ON)
        {
            float weight = 0.0f;

            float3 p = { 0.0f, 0.0f, 0.0f };
            float3 n = { 0.0f, 0.0f, 0.0f };

            int index = 0, unInfluenceCount = 0;
            for (index = 0; index < 4; ++index)
            {
                weight = appData.blendWeight[index];
                if (weight <= 0.0f)
                {
                    ++unInfluenceCount;
                }
                p += (weight * mul(appData.position, boneTransforms[appData.blendIndices[index]])).xyz;
                n += (weight * mul(float4(appData.normal.xyz, 0), boneTransforms[appData.blendIndices[index]])).xyz;
            }

            if (unInfluenceCount != index)
            {
                appData.position = float4(p, 1.0f);
                appData.normal = float4(n, 0.0f);
            }
        }

		
        vsOutput.lightDirection = lightDirection;
        vsOutput.materialColor = materialColor;
        vsOutput.normal = appData.normal;

        vsOutput.position = mul(appData.position, worldViewProjection);
        vsOutput.normal.w = 0;
        float4 N = normalize(mul(appData.normal, world));
        float4 L = float4(normalize(lightDirection.xyz), lightDirection.w);
        vsOutput.color = materialColor * appData.color * max(0, -dot(L, N));	
    }


    return vsOutput;
}