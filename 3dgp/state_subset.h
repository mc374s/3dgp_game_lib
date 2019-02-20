#ifndef _STATE_SUBSET_H_
#define _STATE_SUBSET_H_

#include <d3d11.h>

namespace GLC 
{

template<typename D3D_TYPE, size_t MAX>
class StateTemplate {
public:

protected:
	StateTemplate() = default;
	~StateTemplate() = default;

	static D3D_TYPE* pState[MAX];

public:
	// Return a Initialized State Object*
	template<typename MODE_TYPE>
	static D3D_TYPE* Get(MODE_TYPE mode = MAX)
	{
		if (mode >= 0 && mode < MAX) {
			return pState[mode];
		}
		else {
			return nullptr;
		}
	};
	static void Release() 
	{
		for (int i = 0; i < MAX; i++) {
			if (pState[i]) {
				pState[i]->Release();
				pState[i] = nullptr;
			}
		}
	};
};

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


//--------------------------------------------------------------------------------------
// DepthStencilState
//--------------------------------------------------------------------------------------
namespace DepthStencil
{
	enum MODE
	{
		NONE,
		DEFAULT,
		READ,

		MAX
	};
	struct State : public StateTemplate<ID3D11DepthStencilState, MODE::MAX>
	{
		friend void Initialize(ID3D11Device *pDevice);
	private:
		State() = default;
		~State() = default;
	};

};


//--------------------------------------------------------------------------------------
// RasterizerState
//--------------------------------------------------------------------------------------
namespace Rasterizer
{
	enum MODE
	{
		CULL_NONE,
		CULL_CLOCKWISE,
		CULL_COUNTER_CLOCKWISE,
		WIREFRAME,

		MAX
	};
	struct State : public StateTemplate<ID3D11RasterizerState, MODE::MAX>
	{
		static void Initialize(ID3D11Device *pDevice);
	private:
		State() = default;
		~State() = default;
	};

};


}; // namespace GLC

#endif // !_STATE_SUBSET_H_
