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
	UINT m_vertexCount;

	ID3D11RasterizerState*		m_pRasterizerState;
	ID3D11Buffer*				m_pVertexBuffer;
	D3D11_TEXTURE2D_DESC		m_renderTargetTextureDesc;
	ID3D11SamplerState*			m_pSamplerState;
	ID3D11DepthStencilState*	m_pDepthStencilState;

	ID3D11Buffer*				m_pVSProjectionCBuffer;

	ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11PixelShader*			m_pPixelShader;

	bool m_doProjection;


public:

	Sprite(ID3D11Device* a_pDevice);
	Sprite(ID3D11Device* a_pDevice, char* a_pFilename/*Texture file name*/, bool a_doProjection = false);
	~Sprite();
	bool initialize(ID3D11Device* a_pDevice);

	XMFLOAT2 toNDC_UV(XMFLOAT2 a_coord);
	XMFLOAT3 rotationZ(XMFLOAT3 a_coord, float a_angle, XMFLOAT3 a_center);

	void render(ID3D11DeviceContext* a_pDeviceContext);
	void render(ID3D11DeviceContext* a_pDeviceContext, vertex a_pCoordNDC[]);
	void render(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_rotateAngle = 0.0, UINTCOLOR a_blendColor = 0xFFFFFFFF);
	void render(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX = .0f, float a_srcY = .0f, float a_srcWidth = .0f, float a_srcHeight = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, float a_rotateAngle = .0f, bool a_doCenterRotation = true, float a_rotatePosX = .0f, float a_rotatePosY = .0f, bool a_doReflection = false, int a_scaleMode = LEFTTOP);

	void setProjection(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const CUSTOM3D* a_pCustom3D = nullptr);

	void render3D(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX = .0f, float a_srcY = .0f, float a_srcWidth = .0f, float a_srcHeight = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, float a_rotateAngle = .0f, bool a_doCenterRotation = true, float a_rotatePosX = .0f, float a_rotatePosY = .0f, bool a_doReflection = false, int a_scaleMode = LEFTTOP, const CUSTOM3D* _custom3D = nullptr);


};


#endif // ! _SPRITE_H_