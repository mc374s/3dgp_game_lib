#include "state_subset.h"

using namespace GLC;

//--------------------------------------------------------------------------------------
// BlendState
//--------------------------------------------------------------------------------------
ID3D11BlendState* Blend::pBlendState[MAX] = { nullptr };
UINT Blend::oldMode = NONE;

UINT Blend::mode[MAX][6] = {
  {0},//BLEDN_NONE

  { D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_ONE,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_OP_ADD },// BLEND_ALPHA

  { D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_ADD },// BLEND_ADD

  { D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_ZERO,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_OP_REV_SUBTRACT },// BLEND_SUB

  { D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_ZERO,
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_ONE,
	D3D11_BLEND_ZERO,
	D3D11_BLEND_OP_ADD },// BLEND_REPLACE

  { D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_ZERO,
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_ZERO,
	D3D11_BLEND_OP_ADD },// BLEND_MUL

  { D3D11_BLEND_ONE,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_MAX,
	D3D11_BLEND_ONE,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_MAX },// BLEND_LIGHTEN

  { D3D11_BLEND_ONE,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_ONE,
	D3D11_BLEND_ONE,
	D3D11_BLEND_OP_MIN },// BLEND_DARKEN

  { D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_ONE,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_OP_MAX },// BLEND_SCREEN

};

void Blend::Initialize(ID3D11Device *pDevice)
{

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = /*TRUE*/FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	for (int i = 0; i < MAX; i++) {
		blendDesc.RenderTarget[0].SrcBlend = (D3D11_BLEND)mode[i][SRCBLEND];
		blendDesc.RenderTarget[0].DestBlend = (D3D11_BLEND)mode[i][DESTBLEND];
		blendDesc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)mode[i][BLENDOP];
		blendDesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)mode[i][SRCBLENDALPHA];
		blendDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)mode[i][DESTBLENDALPHA];
		blendDesc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)mode[i][BLENDOPALPHA];

		if (blendDesc.RenderTarget[0].SrcBlend != 0) {
			HRESULT hr = pDevice->CreateBlendState(&blendDesc, &pBlendState[i]);
			if (FAILED(hr)) {
				MessageBox(0, L"Blend: Create BlendState failed", 0, 0);
				return;
			}
		}
	}
}

void Blend::SetMode(ID3D11DeviceContext* pDeviceContext, MODE mode)
{
	if (oldMode != mode) {
		oldMode = mode;
		pDeviceContext->OMSetBlendState(pBlendState[mode], nullptr, 0xFFFFFFFF);
	}
}

ID3D11BlendState* Blend::State(MODE blendMode)
{
	if (blendMode >= 0 && blendMode < MAX) {
		return pBlendState[blendMode];
	}
	else {
		return nullptr;
	}
}

void Blend::Release()
{
	for (int i = 0; i < MAX; i++) {
		if (pBlendState[i]) {
			pBlendState[i]->Release();
		}
	}
}



//--------------------------------------------------------------------------------------
// SamplerState
//--------------------------------------------------------------------------------------
ID3D11SamplerState* Sampler::pSamplerState[MAX] = { nullptr };

void Sampler::Initialize(ID3D11Device *pDevice)
{

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDesc.MipLODBias = 0.0f;
	//samplerDesc.MaxAnisotropy = 16;
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	/*samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;*/
	//samplerDesc.MinLOD = 0;
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (!pSamplerState[WRAP]) {
		pDevice->CreateSamplerState(&samplerDesc, &pSamplerState[WRAP]);
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	if (!pSamplerState[MIRROR]) {
		pDevice->CreateSamplerState(&samplerDesc, &pSamplerState[MIRROR]);
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	if (!pSamplerState[CLAMP]) {
		pDevice->CreateSamplerState(&samplerDesc, &pSamplerState[CLAMP]);
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	if (!pSamplerState[BORDER]) {
		pDevice->CreateSamplerState(&samplerDesc, &pSamplerState[BORDER]);
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	if (!pSamplerState[MIRROR_ONCE]) {
		pDevice->CreateSamplerState(&samplerDesc, &pSamplerState[MIRROR_ONCE]);
	}


}

ID3D11SamplerState* Sampler::State(MODE samplerMode)
{
	if (samplerMode >= 0 && samplerMode < MAX) {
		return pSamplerState[samplerMode];
	}
	else {
		return nullptr;
	}
}

void Sampler::Release()
{
	for (int i = 0; i < MAX; i++) {
		if (pSamplerState[i]) {
			pSamplerState[i]->Release();
		}
	}
}


//--------------------------------------------------------------------------------------
// DepthStencilState
//--------------------------------------------------------------------------------------
ID3D11DepthStencilState* DepthStencil::State::pState[MAX];

void DepthStencil::State::Initialize(ID3D11Device *pDevice)
{
	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &State::pState[NONE]);

	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &State::pState[DEFAULT]);

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &State::pState[READ]);
	
}

//--------------------------------------------------------------------------------------
// RasterizerState
//--------------------------------------------------------------------------------------
ID3D11RasterizerState* Rasterizer::State::pState[MAX];

void Rasterizer::State::Initialize(ID3D11Device* pDevice)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = TRUE;

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	pDevice->CreateRasterizerState(&rasterizerDesc, &State::pState[CULL_NONE]);

	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	pDevice->CreateRasterizerState(&rasterizerDesc, &State::pState[CULL_CLOCKWISE]);

	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	pDevice->CreateRasterizerState(&rasterizerDesc, &State::pState[CULL_COUNTER_CLOCKWISE]);

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	pDevice->CreateRasterizerState(&rasterizerDesc, &State::pState[WIREFRAME]);
}


void test() {
	DepthStencil::State::Get(DepthStencil::DEFAULT);
	//Get(DepthStencil::DEFAULT);
}