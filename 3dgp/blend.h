#ifndef _BLEND_H_
#define _BLEND_H_

#include <d3d11.h>

#define BLEND_MODE_MAX (9)

enum BLEND_MODE
{
	BLEND_NONE = 0,
	BLEND_ALPHA,
	BLEND_ADD,
	BLEND_SUB,
	BLEND_REPLACE,
	BLEND_MUL,
	BLEND_LIGHTEN,
	BLEND_DARKEN,
	BLEND_SCREEN,
};

class MyBlending
{
private:
	static ID3D11BlendState* s_pblendState[BLEND_MODE_MAX];
	static int s_blendMode[BLEND_MODE_MAX][6];
	static int s_oldMode;
	MyBlending() {};
	~MyBlending() {};

public:
	enum RenderTargetElement
	{
		SRCBLEND,
		DESTBLEND,
		BLENDOP,
		SRCBLENDALPHA,
		DESTBLENDALPHA,
		BLENDOPALPHA,
	};

	static void Initialize(ID3D11Device *pDevice);
	static void setMode(ID3D11DeviceContext* &pDeviceContext, int mode = BLEND_NONE);
	static void Release();
};


#endif // !_BLEND_H_
