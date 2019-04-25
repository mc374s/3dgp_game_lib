#include "common.hlsli"

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

};

#define MAX_BONES (32)
cbuffer PerObjectBoneBlend : register(b3)
{
    row_major matrix boneTransforms[MAX_BONES];
};



VSOUT diffuse(VSIN_Color appData)
{
	VSOUT vsOut;

	vsOut.position = appData.position;
	vsOut.diffuse = appData.color;

	return vsOut;
}


VSOUT_Tex diffuse_tex(VSIN_NormalColorTex appData)
{
	VSOUT_Tex vsOut;

	vsOut.position = appData.position;
	vsOut.diffuse = appData.color;
	vsOut.texcoord = appData.texcoord; 

	return vsOut;
}


VSOUT diffuse_normal(VSIN_ColorNormal appData)
{
	VSOUT vsOut;

	appData.position.z = 1;
	appData.normal.w = 0;

	vsOut.position = mul(appData.position, worldViewProjection);
	float4 N = normalize(mul(appData.normal, world));
    float4 L = float4(normalize(lightDirection.xyz), lightDirection.w);
    vsOut.diffuse = materialColor * appData.color * max(0, -dot(L, N));
	vsOut.diffuse.a = appData.color.a;
	
	return vsOut;	
}


VSOUT_Tex diffuse_tex_normal(VSIN_NormalColorTex appData)
{
	VSOUT_Tex vsOut;

	appData.position.z = 1;
	appData.normal.w = 0;

	vsOut.position = mul(appData.position, worldViewProjection);
	float4 N = normalize(mul(appData.normal, world));
    float4 L = float4(normalize(lightDirection.xyz), lightDirection.w);
    vsOut.diffuse = materialColor * appData.color * max(0, -dot(L, N));
	vsOut.diffuse.a = appData.color.a;
	vsOut.texcoord = appData.texcoord;

	return vsOut;
	
}


VSOUT_Tex diffuse_tex_normal_bone(VSIN_NormalColorTexWeight appData)
{
	VSOUT_Tex vsOut;

	appData.position.z = 1;
	appData.normal.z = 0;

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
        n += (weight * mul(appData.normal, boneTransforms[appData.blendIndices[index]])).xyz;
    }

    if (unInfluenceCount != index)
    {
        appData.position.xyz = p;
        appData.normal.xyz = n;
    }
	

	vsOut.position = mul(appData.position, worldViewProjection);
	float4 N = normalize(mul(appData.normal, world));
    float4 L = float4(normalize(lightDirection.xyz), lightDirection.w);
    vsOut.diffuse = materialColor * appData.color * max(0, -dot(L, N));
	vsOut.diffuse.a = appData.color.a;
	vsOut.texcoord = appData.texcoord;

	return vsOut;
}

