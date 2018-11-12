#include "texture_on.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float4 normal : NORMAL, float4 boneWeights : WEIGHTS, uint4 boneIndices : BONES)
{
	float4 influence = { 0.0f,0.0f,0.0f,1.0f };
	float weight;

	float3 p = { 0.0f,0.0f,0.0f };
	float3 n = { 0.0f,0.0f,0.0f };

	int boneIndex = 0, unInfluenceCount = 0;
	for (boneIndex = 0; boneIndex  < 4; ++boneIndex) {
		weight = boneWeights[boneIndex];
		if (weight > 0.0f) {
			switch (boneIndices[boneIndex]) {
			case 0: influence.r = weight; break;
			case 1: influence.g = weight; break;
			case 2: influence.b = weight; break;
			default:break;
			}
		}
		else {
			++unInfluenceCount;
		}
		p += (boneWeights[boneIndex] * mul(position, boneTransforms[boneIndices[boneIndex]])).xyz;
		n += (boneWeights[boneIndex] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[boneIndex]])).xyz;
	}

	if (unInfluenceCount != boneIndex) {
		position = float4(p, 1.0f);
		normal = float4(n, 0.0f);
		//influence.r = 1.0f;
		//influence.g = 1.0f;
		//influence.b = 1.0f;
	}

	VS_OUT vout;
	vout.texcoord = texcoord;


	vout.position = mul(position, worldViewProjection);

    normal.w = 0;
	float4 N = normalize(mul(normal, world));
    float4 L = float4(normalize(lightDirection.xyz), lightDirection.w);

	//vout.color = materialColor * max(0, -dot(L, N));
	//vout.color.a = materialColor.a;

    vout.color = materialColor * color * max(0, -dot(L, N));
	vout.color.a = color.a;

	if (unInfluenceCount != boneIndex) {
		//vout.color = influence;
		//influence.r = 1.0f;
		//influence.g = 1.0f;
		//influence.b = 1.0f;
	}




	return vout;
}