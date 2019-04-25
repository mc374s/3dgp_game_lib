// Vertex shader Input
struct VSIN
{
    float4 position : SV_Position;
};

struct VSIN_Color
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

struct VSIN_ColorTex
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VSIN_ColorNormal
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float4 normal : NORMAL;
};

struct VSIN_NormalColorTex
{
    float4 position : SV_Position;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VSIN_NormalColorTexWeight
{
    float4 position : SV_Position;
    float4 normal : NORMAL;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD0;
	
    uint blendIndices[4] : BLENDINDICES;
    float blendWeight[4] : BLENDWEIGHT;
};


// Vertex shader Output
struct VSOUT
{
	float4 position : SV_Position;
    float4 diffuse : COLOR;
};

struct VSOUT_Tex
{
	float4 position : SV_Position;
    float4 diffuse : COLOR;
    float2 texcoord : TEXCOORD0;
};

struct VSOUT_NormalTex
{
	float4 position : SV_Position;
    float4 diffuse : COLOR;
    float2 texcoord : TEXCOORD0;
    float4 normal : TEXCOORD1;
};


// Pixel shader Input
struct PSIN
{
    float4 diffuse : COLOR;
};

struct PSIN_Tex
{
    float4 diffuse : COLOR;
    float2 texcoord : TEXCOORD0;
};

struct PSIN_NormalTex
{
    float4 diffuse : COLOR;
    float2 texcoord : TEXCOORD0;
    float4 normal : TEXCOORD1;
};