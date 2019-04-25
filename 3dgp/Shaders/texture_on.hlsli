struct VS_OUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 texcoord: TEXCOORD;
};

#define MAX_BONES (32)

cbuffer PROJECTION_CBUFFER : register(b3)
{
	row_major matrix world;
	row_major matrix view;
	row_major matrix projection;
	row_major matrix worldViewProjection;
	float4 materialColor;
	float4 lightDirection;
	
	row_major matrix boneTransforms[MAX_BONES];
};