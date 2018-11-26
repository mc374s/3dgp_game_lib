#include "obj2d.h"

/////////////////////////////////////////////////////////////////
// Class OBJ2D Function
OBJ2D::OBJ2D() 
{
	Clear();
}

void OBJ2D::MemberCopy(const OBJ2D& inputObj)
{
	pos = inputObj.pos;
	initPos = inputObj.initPos;
	setPos = inputObj.setPos;
	speed = inputObj.speed;
	speedAcc = inputObj.speedAcc;
	speedMax = inputObj.speedMax;

	size = inputObj.size;

	transform2D = inputObj.transform2D;
	transform = inputObj.transform;

	timer = inputObj.timer;
	step = inputObj.step;
	alpha = inputObj.alpha;
	type = inputObj.type;

	isInit = inputObj.isInit;

	pSprData = inputObj.pSprData;
	pfMove = inputObj.pfMove;

	setAlpha = inputObj.setAlpha;
	isHitable = inputObj.isHitable;

	//memcpy(this, &inputObj, sizeof(OBJ2D));
	//pSprData = inputObj.pSprData;
	//pfMove = inputObj.pfMove;
}

OBJ2D::OBJ2D(const OBJ2D& inputObj)
{
	MemberCopy(inputObj);
}

OBJ2D::~OBJ2D() 
{
	Clear();
}

const OBJ2D& OBJ2D::operator=(const OBJ2D& right)
{
	MemberCopy(right);
	return *this;
}


void OBJ2D::Clear() 
{
	pfMove = nullptr;
	pSprData = nullptr;
	pos = initPos = setPos = Vector3(0, 0, 0);
	speed = speedAcc = speedMax = size = Vector3(0, 0, 0);
	timer = 0;
	step = 0;
	transform2D.Clear();
	transform.Clear();

	alpha = 255;
	isInit = false;

	type = 0;
	setAlpha = 255;
}

void OBJ2D::Update()
{
	if (pfMove){
		(this->*pfMove)();
	}
	//pos.y = initPos.y - SCROLL_Y;
}

void OBJ2D::Draw() 
{
	if (pSprData)
	{
		if (alpha > 255) {
			alpha = 255;
		}
		if (alpha < 0) {
			alpha = 0;
		}
		transform2D.rgba = transform2D.rgba >> 8 << 8 | alpha;
		//pSprData->draw(pos.x, pos.y, &transform2D);
		pSprData->Draw(pos, transform2D, transform.position, transform.rotationDegree);
	}
}

int OBJ2D::SearchSet(OBJ2D** ppBegin, int max) 
{
	for (int i = 0; i < max; i++)
	{
		if (ppBegin[i] && ppBegin[i]->isInit) {
			continue;
		}
		return i;
	}
	return -1;
}


///////////////////////////////////////////////////////////////
// Class OBJ2DEX Function

OBJ2DEX::OBJ2DEX()
{
	Clear();
}

void OBJ2DEX::MemberCopy(const OBJ2DEX& inputObj)
{
	OBJ2D::MemberCopy(inputObj);
	aframe = inputObj.aframe;
	animeNO = inputObj.animeNO;
	animeCounter = inputObj.animeCounter;
	pAnimeData = inputObj.pAnimeData;
}

OBJ2DEX::OBJ2DEX(const OBJ2DEX& inputObj):OBJ2D(inputObj)
{
	MemberCopy(inputObj);
}

OBJ2DEX::~OBJ2DEX()
{
	Clear();
}

const OBJ2DEX& OBJ2DEX::operator=(const OBJ2DEX& right)
{
	MemberCopy(right);
	return *this;
}

void OBJ2DEX::Clear() 
{
	OBJ2D::Clear();
	aframe = 0;
	animeNO = 0;
	animeCounter = 0;
	pAnimeData = nullptr;
}

void OBJ2DEX::animation() 
{
	if (pAnimeData)
	{
		pSprData = &(pAnimeData[animeNO]);
		++aframe;
		if (aframe > (pAnimeData[animeNO]).frameNum)
		{
			aframe = 0;
			animeNO++;
			if ((pAnimeData[animeNO]).texNO < 0)
			{
				++animeCounter;
				animeNO = 0;
			}
		}
	}
}

void OBJ2DEX::Update()
{
	OBJ2D::Update();
}

void OBJ2DEX::Draw() 
{
	OBJ2D::Draw();
	/*if (pSprData)
	{
		if (alpha > 255) {
			alpha = 255;
		}
		if (alpha < 0) {
			alpha = 0;
		}
		transform2D.rgba = transform2D.rgba >> 8 << 8 | alpha;
		pSprData->Draw(pos, &transform2D);
	}*/
}

/////////////////////////////////////////////////////////////////
// Class ObjManager Function
ObjManager::~ObjManager() 
{

}

void ObjManager::Init() 
{

}

void ObjManager::Update(int liveInPagination) {

	// 描画順番を並び替え　pos.z : 小さい順から描画していく 
	//OBJ2D* temp = nullptr;

	//for (int i = 1; i < OBJ_MAX_NUM; i++)
	//{
	//	if (ppObjs[i - 1] && ppObjs[i] && (ppObjs[i - 1]->liveInPagination == ppObjs[i]->liveInPagination))
	//	{
	//		if (ppObjs[i - 1]->pos.z > ppObjs[i]->pos.z)
	//		{
	//			int j = i;
	//			do
	//			{
	//				temp = ppObjs[j - 1];
	//				ppObjs[j - 1] = ppObjs[j];
	//				ppObjs[j] = temp;
	//				j--;
	//			} while (j > 0 && ppObjs[j - 1]->pos.z < ppObjs[j]->pos.z);
	//		}
	//	}
	//}

}

void ObjManager::Draw(int liveInPagination)
{

}
//////////////////////////////////////////////////////////////////