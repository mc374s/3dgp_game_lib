#include "sprite_string.h"
#include "sprite.h"
#include "texture.h"

#include "system.h"

using namespace DirectX;
using namespace GLC;

Sprite* SpriteString::asciiFontSprite = nullptr;
Texture* SpriteString::defaultAsciiFont = nullptr;

void SpriteString::Initialize(char *pFilename)
{
	// Create default ascii font texture if @pFilename exist,
	// else use default ascii font file.
	if (pFilename) {
		defaultAsciiFont = new Texture(pFilename);
	}
	else {
		defaultAsciiFont = new Texture("./Data/Fonts/font5.png");
	}
	defaultAsciiFont->Load();
	SpriteString::asciiFontSprite = new Sprite(GLC::System::pImmediateContext);
}

void XM_CALLCONV SpriteString::DrawString(int drawX, int drawY, char* pIn, FXMVECTOR blendColor, int format, int sizeX, int sizeY, float rotateAngle, Texture* pAsciiFont)
{
	// Calculate clip rectangle and transformation
	XMFLOAT4 clipRect(32, 32, 32, 32);
	XMFLOAT3 position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0);
	//XMVECTOR position{ 0,0,0,0 };
	
	int adjustX = 0, marginX = -sizeX*0.4f;
	if (pIn && asciiFontSprite) {
		if (format == STR_CENTER)
		{
			if (strchr(pIn, '\n'))
			{
				adjustX = -(strchr(pIn, '\n') - pIn)*(sizeX + marginX) / 2;
			}
			else
			{
				adjustX = -((int)strlen(pIn))*(sizeX + marginX) / 2;
			}
		}

		// If custom ascii font texture exist, use it, else use preset ascii font texture;
		Texture* targetAsciiFont = pAsciiFont ? pAsciiFont : defaultAsciiFont;

		// Traverse input char array, calculate every character's screen position and clip position.
		for (int i = 0, j = 0; pIn[i] != '\0'; i++, j++)
		{
			if (pIn[i] == '\n')
			{
				drawY += sizeY * 1;
				j = -1;
				continue;
			}
			clipRect.x = (pIn[i] - (pIn[i] >> 0x4 << 0x4)) << 0x5;
			clipRect.y = pIn[i] >> 0x4 << 0x5;

			//position = XMVectorSetX(position, drawX + j * (sizeX + marginX) + adjustX);
			//position = XMVectorSetY(position, drawY);

			position.x = drawX + j * (sizeX + marginX) + adjustX;
			position.y = drawY;
			scale.x = sizeX / clipRect.z;
			scale.y = sizeY / clipRect.w;
			rotation.z = rotateAngle;
			asciiFontSprite->Draw(targetAsciiFont, XMLoadFloat4(&clipRect), XMLoadFloat3(&position), XMLoadFloat3(&scale), XMLoadFloat3(&rotation), blendColor);

			// Old Sprite Draw method
			//SpriteString::asciiFontSprite->Draw(drawX + j * (sizeX + marginX) + adjustX, drawY, sizeX, sizeY, (pIn[i] - (pIn[i] >> 0x4 << 0x4)) << 0x5, pIn[i] >> 0x4 << 0x5, 32.0f, 32.0f, blendColor, rotateAngle);
		}

	}
}

void SpriteString::Release()
{
	if (SpriteString::asciiFontSprite) {
		delete SpriteString::asciiFontSprite;
	}
	if (defaultAsciiFont) {
		delete defaultAsciiFont;
	}
}