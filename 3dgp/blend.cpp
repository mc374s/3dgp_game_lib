#include "blend.h"

ID3D11BlendState* MyBlending::s_pblendState[BLEND_MODE_MAX] = { NULL };
int MyBlending::s_oldMode = BLEND_NONE;

int MyBlending::s_blendMode[BLEND_MODE_MAX][6] = {
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

void MyBlending::initialize(ID3D11Device *a_pDevice)
{

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = /*TRUE*/FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	for (int i = 0; i < BLEND_MODE_MAX; i++)
	{
		blendDesc.RenderTarget[0].SrcBlend			= (D3D11_BLEND)s_blendMode[i][SRCBLEND];
		blendDesc.RenderTarget[0].DestBlend			= (D3D11_BLEND)s_blendMode[i][DESTBLEND];
		blendDesc.RenderTarget[0].BlendOp			= (D3D11_BLEND_OP)s_blendMode[i][BLENDOP];
		blendDesc.RenderTarget[0].SrcBlendAlpha		= (D3D11_BLEND)s_blendMode[i][SRCBLENDALPHA];
		blendDesc.RenderTarget[0].DestBlendAlpha	= (D3D11_BLEND)s_blendMode[i][DESTBLENDALPHA];
		blendDesc.RenderTarget[0].BlendOpAlpha		= (D3D11_BLEND_OP)s_blendMode[i][BLENDOPALPHA];

		if (blendDesc.RenderTarget[0].SrcBlend != 0)
		{
			HRESULT hr = a_pDevice->CreateBlendState(&blendDesc, &s_pblendState[i]);
			if (FAILED(hr))
			{
				MessageBox(0, L"Blend: Create BlendState failed", 0, 0);
				return;
			}
		}
	}
}

void MyBlending::setMode(ID3D11DeviceContext* &a_pDeviceContext, int a_mode)
{
	if (s_oldMode != a_mode)
	{
		s_oldMode = a_mode;
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		a_pDeviceContext->OMSetBlendState(s_pblendState[a_mode], blendFactor, 0xFFFFFFFF);
	}
}

void MyBlending::release()
{
	for (int i = 0; i < BLEND_MODE_MAX; i++)
	{
		if (s_pblendState[i])
		{
			s_pblendState[i]->Release();
		}
	}
}

