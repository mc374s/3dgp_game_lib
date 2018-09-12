#include "game.h"
#include "obj3d.h"

/////////////////////////////////////////////////////////////////
// Class OBJ3D Function
OBJ3D::OBJ3D()
{
	clear();
}

void OBJ3D::memberCopy(const OBJ3D& inputObj)
{
	initPos = inputObj.initPos;
	setPos = inputObj.setPos;
	speed = inputObj.speed;
	speedAcc = inputObj.speedAcc;
	speedMax = inputObj.speedMax;

	size = inputObj.size;

	transform = inputObj.transform;

	timer = inputObj.timer;
	step = inputObj.step;
	type = inputObj.type;

	isInit = inputObj.isInit;

	meshData = inputObj.meshData;
	moveFunc = inputObj.moveFunc;

	isHitAble = inputObj.isHitAble;
}

OBJ3D::OBJ3D(const OBJ3D& inputObj)
{
	memberCopy(inputObj);
}

OBJ3D::~OBJ3D()
{
	clear();
}

const OBJ3D& OBJ3D::operator=(const OBJ3D& rhv)
{
	memberCopy(rhv);
	return *this;
}


void OBJ3D::clear()
{
	moveFunc = nullptr;
	meshData = nullptr;
	speed = speedAcc = speedMax = size = Vector3(0, 0, 0);
	timer = 0;
	step = 0;
	transform.clear();

	isInit = false;

	type = 0;
}

void OBJ3D::update()
{
	if (moveFunc) {
		(this->*moveFunc)();
	}
	//pos.y = initPos.y - SCROLL_Y;
}

void OBJ3D::draw()
{
	if (meshData)
	{
		meshData->draw(transform);
	}

}

int OBJ3D::searchSet(OBJ3D** begin, int max)
{
	for (int i = 0; i < max; i++)
	{
		if (begin[i] && begin[i]->isInit) {
			continue;
		}
		return i;
	}
	return -1;
}