#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <d3d11.h>
#include <DirectXMath.h>

enum SCALE_MODE
{
	LEFTTOP,
	TOPCENTER,
	RIGHTTOP,
	RIGHTCENTER,
	RIGHTBOTTOM,
	BOTTOMCENTER,
	LEFTBOTTOM,
	LEFTCENTER,
	CENTER,
};


class Sprite
{
private:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
		DirectX::XMFLOAT4 normal;
	};

	struct PROJECTION_CBUFFER
	{
		// TODO : Change to XMFLOAT4X4
		DirectX::XMMATRIX world;				//���[���h�ϊ��s��
		DirectX::XMMATRIX view;					//�r���[�ϊ��s��
		DirectX::XMMATRIX projection;			//�v���W�F�N�V�����ϊ��s��
		DirectX::XMMATRIX worldViewProjection;	//���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4 materialColor;		//�ގ��F
		DirectX::XMFLOAT4 lightDirection;		//���C�g�i�s�s��
	}updateCbuffer;

private:

	HRESULT hr;
	UINT vertexCount;

	ID3D11RasterizerState*		pRasterizerState;
	ID3D11Buffer*				pVertexBuffer;
	D3D11_TEXTURE2D_DESC		renderTargetTextureDesc;
	ID3D11SamplerState*			pSamplerState;
	ID3D11DepthStencilState*	pDepthStencilState;

	ID3D11Buffer*				pVSProjectionCBuffer;

	ID3D11ShaderResourceView*	pShaderResourceView;
	ID3D11VertexShader*			pVertexShader;
	ID3D11InputLayout*			pInputLayout;
	ID3D11PixelShader*			pPixelShader;

	bool doProjection;

	bool Initialize(ID3D11Device* pDevice);

	DirectX::XMFLOAT3 RotationZ(DirectX::XMFLOAT3 coord, float angle, DirectX::XMFLOAT3 center);

public:

	Sprite(ID3D11Device* pDevice);
	Sprite(ID3D11Device* pDevice, char* pFilename/*Texture file name*/, bool doProjection = false);
	~Sprite();

	void Draw(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[]);
	void XM_CALLCONV Draw(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float rotateAngle = 0.0, DirectX::FXMVECTOR blendColor = DirectX::g_XMOne);
	void XM_CALLCONV Draw(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, DirectX::FXMVECTOR blendColor = DirectX::g_XMOne, float rotateAngle = .0f, bool doCenterRotation = true, float rotatePosX = .0f, float rotatePosY = .0f, bool doReflection = false, int scaleMode = LEFTTOP);

	void XM_CALLCONV Draw(ID3D11DeviceContext *pDeviceContext, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, 
		float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, DirectX::FXMVECTOR blendColor = DirectX::g_XMOne,
		float rotateAngle = .0f, bool doCenterRotation = true, float rotatePosX = .0f, float rotatePosY = .0f, bool doReflection = false, int scaleMode = LEFTTOP);
};


#endif // ! _SPRITE_H_