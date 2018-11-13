#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

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

	XMFLOAT3 RotationZ(XMFLOAT3 coord, float angle, XMFLOAT3 center);

	bool Initialize(ID3D11Device* pDevice);
	void Draw(ID3D11DeviceContext* pDeviceContext);
	void Draw(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[]);

public:

	RenderTarget(ID3D11Device* pDevice, int renderWidth = 512, int renderHeight = 512);
	~RenderTarget();

	void Draw(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, FXMVECTOR blendColor = g_XMOne, bool doReflection = false);

	void XM_CALLCONV Draw(ID3D11DeviceContext *pDeviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, FXMVECTOR blendColor = g_XMOne, bool doReflection = false);
};





#endif // !_RENDER_TARGET_H_
