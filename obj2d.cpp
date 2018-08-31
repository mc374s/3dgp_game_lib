#include "game.h"
#include "obj2d.h"

#include "sprite_data.h"

/////////////////////////////////////////////////////////////////
// Class OBJ2D Function
OBJ2D::OBJ2D() 
{
	clear();
}

void OBJ2D::memberCopy(const OBJ2D& a_inputObj)
{
	m_pos = a_inputObj.m_pos;
	m_initPos = a_inputObj.m_initPos;
	m_setPos = a_inputObj.m_setPos;
	m_speed = a_inputObj.m_speed;
	m_speedAcc = a_inputObj.m_speedAcc;
	m_speedMax = a_inputObj.m_speedMax;

	m_size = a_inputObj.m_size;

	m_custom = a_inputObj.m_custom;
	m_custom3d = a_inputObj.m_custom3d;

	m_timer = a_inputObj.m_timer;
	m_step = a_inputObj.m_step;
	m_alpha = a_inputObj.m_alpha;
	m_type = a_inputObj.m_type;
	m_concentration = a_inputObj.m_concentration;

	m_isInit = a_inputObj.m_isInit;
	m_liveInPagination = a_inputObj.m_liveInPagination;

	m_pSprData = a_inputObj.m_pSprData;
	m_pfMove = a_inputObj.m_pfMove;

	m_setAlpha = a_inputObj.m_setAlpha;
	m_isHitAble = a_inputObj.m_isHitAble;
}

OBJ2D::OBJ2D(const OBJ2D& a_inputObj)
{
	memberCopy(a_inputObj);
}

OBJ2D::~OBJ2D() 
{
	clear();
}

const OBJ2D& OBJ2D::operator=(const OBJ2D& a_right)
{
	memberCopy(a_right);
	return *this;
}


void OBJ2D::clear() 
{
	m_pfMove = nullptr;
	m_pSprData = nullptr;
	m_pos = m_initPos = m_setPos = Vector3(0, 0, 0);
	m_speed = m_speedAcc = m_speedMax = m_size = Vector3(0, 0, 0);
	m_timer = 0;
	m_step = 0;
	m_custom.clear();
	m_custom3d.clear();

	m_alpha = 255;
	m_concentration = 0;
	m_isInit = false;

	m_liveInPagination = 1;

	m_type = 0;
	m_setAlpha = 255;
}

void OBJ2D::update()
{
	if (m_pfMove){
		(this->*m_pfMove)();
	}
	m_pos.y = m_initPos.y - SCROLL_Y;
}

void OBJ2D::draw() 
{
	if (m_pSprData)
	{
		if (m_alpha > 255) {
			m_alpha = 255;
		}
		if (m_alpha < 0) {
			m_alpha = 0;
		}
		m_custom.rgba = m_custom.rgba >> 8 << 8 | m_alpha;
		//m_pSprData->draw(m_pos.x, m_pos.y, &m_custom);
		m_pSprData->draw(m_pos, &m_custom, &m_custom3d);
	}

}

int OBJ2D::searchSet(OBJ2D** a_ppBegin, int a_max) 
{
	for (int i = 0; i < a_max; i++)
	{
		if (a_ppBegin[i] && a_ppBegin[i]->m_isInit) {
			continue;
		}
		return i;
	}
	return -1;
}

void OBJ2D::blur()
{
	switch (m_step)
	{
	case STEP::INIT:
		m_isHitAble = false;
		m_timer = 0;
		m_step = STEP::BEGIN;
		//m_setAlpha = m_alpha - 15;
		m_custom.scaleMode = SCALE_MODE::CENTER;
		m_custom.scaleY = m_custom.scaleX = 0.7f;
		//m_speed.x = (rand() / (float)RAND_MAX - 0.5f) / 10.0f;
		//m_speed.y = (rand() / (float)RAND_MAX - 0.5f) / 10.0f;
		//m_alpha = 0;
		//break;
	case STEP::BEGIN:
		m_timer++;
		//m_alpha -= 1;
		//if (m_alpha < m_setAlpha)
		//{
		//	m_alpha = m_setAlpha;
		//}
		//m_pos += m_speed;
		m_custom.scaleX += 0.01f;
		if (m_custom.scaleX > 1.5f) {
			m_custom.scaleX = 1.5f;
			m_step = STEP::END;
		}
		m_custom.scaleY = m_custom.scaleX;
		if (m_timer > 30) {
			m_isHitAble = true;
		}
		break;
	case STEP::END:
		break;
	case STEP::FINISH:
		break;
	default:
		break;
	}
}


///////////////////////////////////////////////////////////////
// Class OBJ2DEX Function

OBJ2DEX::OBJ2DEX()
{
	clear();
}

void OBJ2DEX::memberCopy(const OBJ2DEX& a_inputObj)
{
	OBJ2D::memberCopy(a_inputObj);
	m_aframe = a_inputObj.m_aframe;
	m_animeNO = a_inputObj.m_animeNO;
	m_animeCounter = a_inputObj.m_animeCounter;
	m_pAnimeData = a_inputObj.m_pAnimeData;
}

OBJ2DEX::OBJ2DEX(const OBJ2DEX& a_inputObj):OBJ2D(a_inputObj)
{
	memberCopy(a_inputObj);
}

OBJ2DEX::~OBJ2DEX()
{
	clear();
}

const OBJ2DEX& OBJ2DEX::operator=(const OBJ2DEX& a_right)
{
	memberCopy(a_right);
	return *this;
}

void OBJ2DEX::clear() 
{
	OBJ2D::clear();
	m_aframe = 0;
	m_animeNO = 0;
	m_animeCounter = 0;
	m_pAnimeData = nullptr;
}

void OBJ2DEX::animation() 
{
	if (m_pAnimeData)
	{
		m_pSprData = &(m_pAnimeData[m_animeNO]);
		++m_aframe;
		if (m_aframe > (m_pAnimeData[m_animeNO]).frameNum)
		{
			m_aframe = 0;
			m_animeNO++;
			if ((m_pAnimeData[m_animeNO]).texNum < 0)
			{
				++m_animeCounter;
				m_animeNO = 0;
			}
		}
	}
}

void OBJ2DEX::update()
{
	OBJ2D::update();
}

void OBJ2DEX::draw() 
{
	OBJ2D::draw();
	/*if (m_pSprData)
	{
		if (m_alpha > 255) {
			m_alpha = 255;
		}
		if (m_alpha < 0) {
			m_alpha = 0;
		}
		m_custom.rgba = m_custom.rgba >> 8 << 8 | m_alpha;
		m_pSprData->draw(m_pos, &m_custom);
	}*/
}

/////////////////////////////////////////////////////////////////
// Class ObjManager Function
ObjManager::~ObjManager() 
{

}

void ObjManager::init() 
{

}

void ObjManager::update(int a_liveInPagination) {

	// 描画順番を並び替え　pos.z : 小さい順から描画していく 
	//OBJ2D* temp = nullptr;

	//for (int i = 1; i < OBJ_MAX_NUM; i++)
	//{
	//	if (m_ppObjs[i - 1] && m_ppObjs[i] && (m_ppObjs[i - 1]->m_liveInPagination == m_ppObjs[i]->m_liveInPagination))
	//	{
	//		if (m_ppObjs[i - 1]->m_pos.z > m_ppObjs[i]->m_pos.z)
	//		{
	//			int j = i;
	//			do
	//			{
	//				temp = m_ppObjs[j - 1];
	//				m_ppObjs[j - 1] = m_ppObjs[j];
	//				m_ppObjs[j] = temp;
	//				j--;
	//			} while (j > 0 && m_ppObjs[j - 1]->m_pos.z < m_ppObjs[j]->m_pos.z);
	//		}
	//	}
	//}

}

void ObjManager::draw(int a_liveInPagination)
{

}
//////////////////////////////////////////////////////////////////