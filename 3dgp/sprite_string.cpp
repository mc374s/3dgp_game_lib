#include "sprite_string.h"

Sprite* SpriteString::s_pSprString = nullptr;

Sprite* initSprString(ID3D11Device* a_pDevice, char *a_pFilename)
{
	if (a_pFilename)
	{
		SpriteString::s_pSprString = new Sprite(a_pDevice, a_pFilename);
	}
	else
	{
		SpriteString::s_pSprString = new Sprite(a_pDevice, "3dgp/fonts/font5.png");
	}
	return SpriteString::s_pSprString;
}

void drawSprString(ID3D11DeviceContext* a_pDeviceConetxt, int a_drawX, int a_drawY, char* a_pIn, UINTCOLOR a_blendColor, int a_format, int a_sizeX, int a_sizeY, float a_rotateAngle, Sprite *a_pSprStringFont)
{
	int adjustX = 0, marginX = -a_sizeX*0.4;
	if (a_format == STR_CENTER && a_pIn)
	{
		if (strchr(a_pIn, '\n'))
		{
			adjustX = -(strchr(a_pIn, '\n') - a_pIn)*(a_sizeX + marginX) / 2;
		}
		else
		{
			adjustX = -((int)strlen(a_pIn))*(a_sizeX + marginX) / 2;
		}
	}
	if (a_pSprStringFont && a_pIn)
	{
		for (int i = 0, j = 0; a_pIn[i] != '\0'; i++, j++)
		{
			if (a_pIn[i] == '\n')
			{
				a_drawY += a_sizeY * 1;
				j = -1;
				continue;
			}
			a_pSprStringFont->render(a_pDeviceConetxt, a_drawX + j * (a_sizeX + marginX) + adjustX, a_drawY, a_sizeX, a_sizeY, (a_pIn[i] - (a_pIn[i] >> 0x4 << 0x4)) << 0x5, a_pIn[i] >> 0x4 << 0x5, 32.0f, 32.0f, a_blendColor, a_rotateAngle);
		}
	}
	else if (a_pIn)
	{

		for (int i = 0, j = 0; a_pIn[i] != '\0'; i++, j++)
		{
			if (a_pIn[i] == '\n')
			{
				a_drawY += a_sizeY * 1;
				j = -1;
				continue;
			}
			SpriteString::s_pSprString->render(a_pDeviceConetxt, a_drawX + j * (a_sizeX + marginX) + adjustX, a_drawY, a_sizeX, a_sizeY, (a_pIn[i] - (a_pIn[i] >> 0x4 << 0x4)) << 0x5, a_pIn[i] >> 0x4 << 0x5, 32.0f, 32.0f, a_blendColor, a_rotateAngle);
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