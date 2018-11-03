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
	friend Sprite* initSprString(ID3D11Device* pDevice, char *pFilename = nullptr);
	friend void drawSprString(ID3D11DeviceContext* pDeviceConetxt, int drawX = 0, int drawY = 0, char *pIn = nullptr, UINTCOLOR blendColor = 0xFFFFFFFF, int format = STR_LEFT, int sizeX = 32, int sizeY = 32, float rotateAngle = .0f, Sprite *pSprStringFont = nullptr);

	friend void releaseSprString();
};




#endif // !_SPRITESTRING_H_
