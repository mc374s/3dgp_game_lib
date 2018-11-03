#include "sprite_string.h"

Sprite* SpriteString::s_pSprString = nullptr;

Sprite* initSprString(ID3D11Device* pDevice, char *pFilename)
{
	if (pFilename)
	{
		SpriteString::s_pSprString = new Sprite(pDevice, pFilename);
	}
	else
	{
		SpriteString::s_pSprString = new Sprite(pDevice, "3dgp/fonts/font5.png");
	}
	return SpriteString::s_pSprString;
}

void drawSprString(ID3D11DeviceContext* pDeviceConetxt, int drawX, int drawY, char* pIn, UINTCOLOR blendColor, int format, int sizeX, int sizeY, float rotateAngle, Sprite *pSprStringFont)
{
	int adjustX = 0, marginX = -sizeX*0.4;
	if (format == STR_CENTER && pIn)
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
	if (pSprStringFont && pIn)
	{
		for (int i = 0, j = 0; pIn[i] != '\0'; i++, j++)
		{
			if (pIn[i] == '\n')
			{
				drawY += sizeY * 1;
				j = -1;
				continue;
			}
			pSprStringFont->render(pDeviceConetxt, drawX + j * (sizeX + marginX) + adjustX, drawY, sizeX, sizeY, (pIn[i] - (pIn[i] >> 0x4 << 0x4)) << 0x5, pIn[i] >> 0x4 << 0x5, 32.0f, 32.0f, blendColor, rotateAngle);
		}
	}
	else if (pIn)
	{

		for (int i = 0, j = 0; pIn[i] != '\0'; i++, j++)
		{
			if (pIn[i] == '\n')
			{
				drawY += sizeY * 1;
				j = -1;
				continue;
			}
			SpriteString::s_pSprString->render(pDeviceConetxt, drawX + j * (sizeX + marginX) + adjustX, drawY, sizeX, sizeY, (pIn[i] - (pIn[i] >> 0x4 << 0x4)) << 0x5, pIn[i] >> 0x4 << 0x5, 32.0f, 32.0f, blendColor, rotateAngle);
		}
	}
}

void releaseSprString()
{
	if (SpriteString::s_pSprString)
	{
		delete SpriteString::s_pSprString;
	}
}