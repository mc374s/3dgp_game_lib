#ifndef _SPRITESTRING_H_
#define _SPRITESYRING_H_

#include "3dgp_system.h"
#include "sprite.h"

// •¶Žš—ñ•`‰æˆÊ’u
#define	STR_LEFT (0)
#define	STR_CENTER (1)

class SpriteString
{
private:
	SpriteString() {};
	~SpriteString() {};
	static Sprite *s_pSprString;

public:
	friend Sprite* initSprString(ID3D11Device* a_pDevice, char *a_pFilename = nullptr);
	friend void drawSprString(ID3D11DeviceContext* a_pDeviceConetxt, int a_drawX = 0, int a_drawY = 0, char *a_pIn = nullptr, UINTCOLOR a_blendColor = 0xFFFFFFFF, int a_format = STR_LEFT, int a_sizeX = 32, int a_sizeY = 32, float a_rotateAngle = .0f, Sprite *a_pSprStringFont = nullptr);

	friend void releaseSprString();
};




#endif // !_SPRITESTRING_H_
