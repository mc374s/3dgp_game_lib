#ifndef _STATE_SUBSET_H_
#define _STATE_SUBSET_H_

#include <d3d11.h>

namespace GLC {

//--------------------------------------------------------------------------------------
// BlendState
//--------------------------------------------------------------------------------------
class Blend
{
public:
	enum MODE
	{
		NONE,
		ALPHA,
		ADD,
		SUB,
		REPLACE,
		MUL,
		LIGHTEN,
		DARKEN,
		SCREEN,

		MAX
	};

private:
	enum RenderTargetElement
	{
		SRCBLEND,
		DESTBLEND,
		BLENDOP,
		SRCBLENDALPHA,
		DESTBLENDALPHA,
		BLENDOPALPHA,
	};
	static ID3D11BlendState* pBlendState[MAX];
	static UINT mode[MAX][6];
	static UINT oldMode;
	Blend() {};
	~Blend() {};

public:

	static void Initialize(ID3D11Device *pDevice);
	// BlendState will be set here
	static void SetMode(ID3D11DeviceContext* pDeviceContext, MODE mode = ALPHA);
	// Get a Initialized BlendState*
	static ID3D11BlendState* State(MODE blendMode = ALPHA);
	static void Release();
};


//--------------------------------------------------------------------------------------
// SamplerState
//--------------------------------------------------------------------------------------
class Sampler
{
public:
	enum MODE
	{
		NONE,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE,

		MAX
	};

private:
	static ID3D11SamplerState* pSamplerState[MAX];
	Sampler() {};
	~Sampler() {};

public:
	static void Initialize(ID3D11Device *pDevice);
	// Get a Initialized SamplerState*
	static ID3D11SamplerState* State(MODE SamplerMode = WRAP);
	static void Release();
};

// TODO: Complete state_subset

//--------------------------------------------------------------------------------------
// DepethStencilState
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// RasterizerState
//--------------------------------------------------------------------------------------

}; // namespace GLC

#endif // !_STATE_SUBSET_H_
