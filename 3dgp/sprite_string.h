#ifndef _SPRITESTRING_H_
#define _SPRITESYRING_H_

// •¶Žš—ñ•`‰æˆÊ’u
#define	STR_LEFT (0)
#define	STR_CENTER (1)

#include <d3d11.h>
//#define _XM_NO_INTRINSICS_ 
#include <DirectXMath.h>

namespace GLC {

class Texture;
class Sprite;

class SpriteString
{
private:
	SpriteString() {};
	~SpriteString() {};
	static Sprite* asciiFontSprite;
	static Texture* defaultAsciiFont;

public:
	static void Initialize(char *pFilename = nullptr);
	static void XM_CALLCONV DrawString(int drawX = 0, int drawY = 0, char* pIn = nullptr, DirectX::FXMVECTOR blendColor = DirectX::g_XMOne, int format = STR_LEFT, int sizeX = 32, int sizeY = 32, float rotateAngle = .0f, Texture* pAsciiFont = nullptr);

	static void Release();
};

}; // namespace GLC


#endif // !_SPRITESTRING_H_
