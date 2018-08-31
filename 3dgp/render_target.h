#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#define _USE_MATH_DEFINES

#include "3dgp_system.h"

class RenderTarget
{
private:
	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 texcoord;
		XMFLOAT3 normal;
	};

	struct PROJECTION_CBUFFER
	{
		XMMATRIX world;					//ワールド変換行列
		XMMATRIX view;					//ビュー変換行列
		XMMATRIX projection;			//プロジェクション変換行列
		XMMATRIX worldViewProjection;	//ワールド・ビュー・プロジェクション合成行列
		XMFLOAT4 materialColor;			//材質色
		XMFLOAT4 lightDirection;		//ライト進行行列
	};

private:

	HRESULT hr;
	UINT m_vertexCount;

	ID3D11RasterizerState*		m_pRasterizerStateCullBack;
	ID3D11RasterizerState*		m_pRasterizerStateCullFront;

	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pVSProjectionCBuffer;

	D3D11_TEXTURE2D_DESC		m_renderTargetTextureDesc;
	ID3D11Texture2D*			m_pRenderTargetTexture = NULL;
	ID3D11RenderTargetView*		m_pRenderTargetView = NULL;

	ID3D11DepthStencilState*	m_pDepthStencilState;

	ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11PixelShader*			m_pPixelShader;


	int m_virtualWidth = 1280;
	int m_virtualHeight = 720;

public:

	RenderTarget(ID3D11Device* a_pDevice, int a_renderWidth = 512, int a_renderHeight = 512);
	~RenderTarget();
	bool initialize(ID3D11Device* a_pDevice);

	XMFLOAT3 toNDC(float a_screenX, float a_screenY);
	XMFLOAT3 toNDC(XMFLOAT3 a_coord);
	XMFLOAT2 toNDC_UV(XMFLOAT2 a_coord);
	XMFLOAT3 rotationZ(XMFLOAT3 a_coord, float a_angle, XMFLOAT3 a_center);

	void render(ID3D11DeviceContext* a_pDeviceContext);
	void render(ID3D11DeviceContext* a_pDeviceContext, vertex a_pCoordNDC[]);
	void render(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX = .0f, float a_srcY = .0f, float a_srcWidth = .0f, float a_srcHeight = .0f, float a_rotateAngle = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, bool a_doReflection = false);

	void setProjection(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const CUSTOM3D* a_pCustom3D = nullptr);
	void render3D(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX = .0f, float a_srcY = .0f, float a_srcWidth = .0f, float a_srcHeight = .0f, float a_rotateAngle = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, const CUSTOM3D* _custom3D = nullptr, bool a_doReflection = false);
};





#endif // !_RENDER_TARGET_H_
