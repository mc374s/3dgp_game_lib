#include "game.h"
#include "sprite_data.h"

#include "numbers.h"

//Numbers::Numbers()
//{
//	clear();
//}

Numbers::Numbers(SPRITE_DATA* a_pFontData)
{
	clear();
	if (a_pFontData == nullptr)
	{
		m_sprData = e_sprNumbers;
	}
	else
	{
		m_sprData = *a_pFontData;
	}
}

void Numbers::memberCopy(const Numbers& a_inputObj)
{
	m_pos = a_inputObj.m_pos;
	m_speed = a_inputObj.m_speed;
	m_speedAcc = a_inputObj.m_speedAcc;
	m_speedMax = a_inputObj.m_speedMax;

	m_custom = a_inputObj.m_custom;

	m_timer = a_inputObj.m_timer;
	m_step = a_inputObj.m_step;
	m_alpha = a_inputObj.m_alpha;
	m_isVisibleAlways = a_inputObj.m_isVisibleAlways;
	m_isVisible = a_inputObj.m_isVisible;

	m_value = a_inputObj.m_value;
	m_digitNum = a_inputObj.m_digitNum;

	m_sprData = a_inputObj.m_sprData;
}

Numbers::Numbers(const Numbers& a_inputObj)
{
	memberCopy(a_inputObj);
}

Numbers::~Numbers()
{
	clear();
}

const Numbers& Numbers::operator=(const Numbers& a_right)
{
	memberCopy(a_right);
	return *this;
}


void Numbers::clear()
{
	//SAFE_DELETE(m_sprData);
	m_pos = Vector3(0, 0, 0);
	m_speed = m_speedAcc = m_speedMax = Vector3(0, 0, 0);
	m_timer = 0;
	m_step = 0;
	m_alpha = 255;

	m_isVisibleAlways = true;
	m_isVisible = false;
}

void Numbers::setValue(int a_value, Vector3 a_size)
{
	for (m_digitNum = 0; m_digitNum < DIGIT_MAX && a_value > 0; ++m_digitNum)
	{
		digit[m_digitNum].val = a_value % 10;	//  一の位を抽出
		digit[m_digitNum].w = a_size.x;
		digit[m_digitNum].h = a_size.y;
		digit[m_digitNum].x = 0;
		digit[m_digitNum].y = 0;
		a_value /= 10;					//  桁を1つシフト
	}
	--m_digitNum;
	m_custom.scaleX = a_size.x;
	m_custom.scaleY = a_size.y;
	//m_isVisible = true;
}

void Numbers::update()
{

}

void Numbers::draw()
{
	if (m_isVisible)
	{
		if (m_sprData.texNum > 0)
		{
			if (m_alpha > 255) {
				m_alpha = 255;
			}
			if (m_alpha < 0) {
				m_alpha = 0;
			}
			m_custom.rgba = m_custom.rgba >> 8 << 8 | m_alpha;
			for (int i = m_digitNum; i >= 0; --i)
			{
				m_sprData.top = digit[i].val*m_sprData.height;
				m_sprData.draw(m_pos.x + (m_digitNum - i)*digit[i].w*m_sprData.width, m_pos.y, &m_custom);
			}
		}
		if (!m_isVisibleAlways)
		{
			m_isVisible = false;
		}
	}
}