#ifndef _SPRITESTRING_H_
#define _SPRITESYRING_H_

// ������`��ʒu
#define	STR_LEFT (0)
#define	STR_CENTER (1)

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class Sprite;

class SpriteString
{
private:
	SpriteString() {};
	~SpriteString() {};
	static Sprite *s_pSprString;

public:
	static Sprite* Initialize(ID3D11Device* pDevice, char *pFilename = nullptr);
	static void DrawString(ID3D11DeviceContext* pDeviceConetxt, int drawX = 0, int drawY = 0, char *pIn = nullptr, FXMVECTOR blendColor = g_XMOne, int format = STR_LEFT, int sizeX = 32, int sizeY = 32, float rotateAngle = .0f, Sprite *pSprStringFont = nullptr);

	static void Release();
};




#endif // !_SPRITESTRING_H_
