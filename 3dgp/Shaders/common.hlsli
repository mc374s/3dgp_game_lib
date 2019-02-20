struct VertexShaderOutput
{
    float4 position : SV_Position;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
	
    float4 materialColor : TEXCOORD1;
    float4 lightDirection : TEXCOORD2;

    uint psFlag : TEXCOORD3;
	
};

// Pixel shader flags
static const uint PS_TEXTURE_OFF = 1 << 0;
static const uint PS_TEXTURE_ON = 1 << 1;

// Vertex shader flags
static const uint VS_PROJECTION_OFF = 1 << 0;
static const uint VS_PROJECTION_ON = 1 << 1;
static const uint VS_SKINNED_OFF = 1 << 2;
static const uint VS_SKINNED_ON = 1 << 3;