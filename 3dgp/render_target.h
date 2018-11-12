#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "3dgp.h"

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
	UINT vertexCount;

	ID3D11RasterizerState*		pRasterizerStateCullBack;
	ID3D11RasterizerState*		pRasterizerStateCullFront;

	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pVSProjectionCBuffer;

	D3D11_TEXTURE2D_DESC		renderTargetTextureDesc;
	ID3D11Texture2D*			pRenderTargetTexture = nullptr;
	ID3D11RenderTargetView*		pRenderTargetView = nullptr;

	ID3D11DepthStencilState*	pDepthStencilState;

	ID3D11ShaderResourceView*	pShaderResourceView;
	ID3D11VertexShader*			pVertexShader;
	ID3D11InputLayout*			pInputLayout;
	ID3D11PixelShader*			pPixelShader;


	int virtualWidth = 1280;
	int virtualHeight = 720;


	XMFLOAT3 ToNDC(float screenX, float screenY);
	XMFLOAT3 ToNDC(XMFLOAT3 coord);
	XMFLOAT2 ToNDC_UV(XMFLOAT2 coord);
	XMFLOAT3 RotationZ(XMFLOAT3 coord, float angle, XMFLOAT3 center);

	bool Initialize(ID3D11Device* pDevice);
	void Render(ID3D11DeviceContext* pDeviceContext);
	void Render(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[]);

public:

	RenderTarget(ID3D11Device* pDevice, int renderWidth = 512, int renderHeight = 512);
	~RenderTarget();

	void Draw(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);

	void Draw(ID3D11DeviceContext* pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);
};





#endif // !_RENDER_TARGET_H_
