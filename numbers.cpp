#include "game.h"
#include "sprite_data.h"

#include "numbers.h"

//Numbers::Numbers()
//{
//	clear();
//}

Numbers::Numbers(SPRITE_DATA* pFontData)
{
	clear();
	if (pFontData == nullptr)
	{
		sprData = e_sprNumbers;
	}
	else
	{
		sprData = *pFontData;
	}
}

void Numbers::memberCopy(const Numbers& inputObj)
{
	pos = inputObj.pos;
	speed = inputObj.speed;
	speedAcc = inputObj.speedAcc;
	speedMax = inputObj.speedMax;

	transform2D = inputObj.transform2D;

	timer = inputObj.timer;
	step = inputObj.step;
	alpha = inputObj.alpha;
	isVisibleAlways = inputObj.isVisibleAlways;
	isVisible = inputObj.isVisible;

	value = inputObj.value;
	digitNum = inputObj.digitNum;

	sprData = inputObj.sprData;
}

Numbers::Numbers(const Numbers& inputObj)
{
	memberCopy(inputObj);
}

Numbers::~Numbers()
{
	clear();
}

const Numbers& Numbers::operator=(const Numbers& right)
{
	memberCopy(right);
	return *this;
}


void Numbers::clear()
{
	//SAFE_DELETE(sprData);
	pos = Vector3(0, 0, 0);
	speed = speedAcc = speedMax = Vector3(0, 0, 0);
	timer = 0;
	step = 0;
	alpha = 255;

	isVisibleAlways = true;
	isVisible = false;
}

void Numbers::setValue(int value, Vector3 size)
{
	for (digitNum = 0; digitNum < DIGIT_MAX && value > 0; ++digitNum)
	{
		digit[digitNum].val = value % 10;	//  一の位を抽出
		digit[digitNum].w = size.x;
		digit[digitNum].h = size.y;
		digit[digitNum].x = 0;
		digit[digitNum].y = 0;
		value /= 10;					//  桁を1つシフト
	}
	--digitNum;
	transform2D.scaleX = size.x;
	transform2D.scaleY = size.y;
	//isVisible = true;
}

void Numbers::update()
{

}

void Numbers::draw()
{
	if (isVisible)
	{
		if (sprData.texNO > 0)
		{
			if (alpha > 255) {
				alpha = 255;
			}
			if (alpha < 0) {
				alpha = 0;
			}
			transform2D.rgba = transform2D.rgba >> 8 << 8 | alpha;
			for (int i = digitNum; i >= 0; --i)
			{
				sprData.top = digit[i].val*sprData.height;
				sprData.draw(pos.x + (digitNum - i)*digit[i].w*sprData.width, pos.y, transform2D);
			}
		}
		if (!isVisibleAlways)
		{
			isVisible = false;
		}
	}
}