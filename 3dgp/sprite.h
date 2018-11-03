#ifndef _SPRITE_H_
#define _SPRITE_H_

#define _USE_MATH_DEFINES


#include "3dgp_system.h"

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
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 texcoord;
		XMFLOAT4 normal;
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


public:

	Sprite(ID3D11Device* pDevice);
	Sprite(ID3D11Device* pDevice, char* pFilename/*Texture file name*/, bool doProjection = false);
	~Sprite();
	bool initialize(ID3D11Device* pDevice);

	XMFLOAT2 toNDC_UV(XMFLOAT2 coord);
	XMFLOAT3 rotationZ(XMFLOAT3 coord, float angle, XMFLOAT3 center);

	void render(ID3D11DeviceContext* pDeviceContext);
	void render(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[]);
	void render(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float rotateAngle = 0.0, UINTCOLOR blendColor = 0xFFFFFFFF);
	void render(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, float rotateAngle = .0f, bool doCenterRotation = true, float rotatePosX = .0f, float rotatePosY = .0f, bool doReflection = false, int scaleMode = LEFTTOP);

	void setProjection(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const Transform& transform = Transform::initialValue());

	void render3D(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, float rotateAngle = .0f, bool doCenterRotation = true, float rotatePosX = .0f, float rotatePosY = .0f, bool doReflection = false, int scaleMode = LEFTTOP, const Transform& transform = Transform::initialValue());


};


#endif // ! _SPRITE_H_