#include "numbers.h"

#include "sprite_data.h"



Numbers::Numbers(Game::SpriteData* pFontData)
{
	Clear();
	if (pFontData == nullptr)
	{
		sprData = &Game::sprNumbers;
	}
	else
	{
		sprData = pFontData;
	}
}

void Numbers::MemberCopy(const Numbers& inputObj)
{
	speed = inputObj.speed;
	speedAcc = inputObj.speedAcc;
	speedMax = inputObj.speedMax;

	transform = inputObj.transform;

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
	MemberCopy(inputObj);
}

Numbers::~Numbers()
{
	Clear();
}

const Numbers& Numbers::operator=(const Numbers& right)
{
	MemberCopy(right);
	return *this;
}


void Numbers::Clear()
{
	sprData = nullptr;
	transform.Clear();
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
	transform.scale.x = size.x;
	transform.scale.y = size.y;
	//isVisible = true;
}

void Numbers::Update()
{

}

void Numbers::Draw()
{
	if (isVisible)
	{
		if (sprData)
		{
			if (alpha > 255) {
				alpha = 255;
			}
			if (alpha < 0) {
				alpha = 0;
			}
			rgba = rgba >> 8 << 8 | alpha;
			for (int i = digitNum; i >= 0; --i)
			{
				sprData->clipRect.top = digit[i].val*sprData->clipRect.height;
				sprData->offsetX = (digitNum - i)*digit[i].w*sprData->clipRect.width;
				sprData->Draw(transform, rgba);
			}
		}
		if (!isVisibleAlways)
		{
			isVisible = false;
		}
	}
}