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
	UINT vertexCount;

	ID3D11RasterizerState*		pRasterizerStateCullBack;
	ID3D11RasterizerState*		pRasterizerStateCullFront;

	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pVSProjectionCBuffer;

	D3D11_TEXTURE2D_DESC		renderTargetTextureDesc;
	ID3D11Texture2D*			pRenderTargetTexture = NULL;
	ID3D11RenderTargetView*		pRenderTargetView = NULL;

	ID3D11DepthStencilState*	pDepthStencilState;

	ID3D11ShaderResourceView*	pShaderResourceView;
	ID3D11VertexShader*			pVertexShader;
	ID3D11InputLayout*			pInputLayout;
	ID3D11PixelShader*			pPixelShader;


	int virtualWidth = 1280;
	int virtualHeight = 720;

public:

	RenderTarget(ID3D11Device* pDevice, int renderWidth = 512, int renderHeight = 512);
	~RenderTarget();
	bool initialize(ID3D11Device* pDevice);

	XMFLOAT3 toNDC(float screenX, float screenY);
	XMFLOAT3 toNDC(XMFLOAT3 coord);
	XMFLOAT2 toNDC_UV(XMFLOAT2 coord);
	XMFLOAT3 rotationZ(XMFLOAT3 coord, float angle, XMFLOAT3 center);

	void render(ID3D11DeviceContext* pDeviceContext);
	void render(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[]);
	void render(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);

	void setProjection(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const Transform& transform = Transform::initialValue());
	void render3D(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, const Transform& transform = Transform::initialValue(), bool doReflection = false);
};





#endif // !_RENDER_TARGET_H_
