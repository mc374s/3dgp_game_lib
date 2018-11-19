#include "game.h"
#include "obj3d.h"

/////////////////////////////////////////////////////////////////
// Class OBJ3D Function
OBJ3D::OBJ3D()
{
	Clear();
}

void OBJ3D::memberCopy(const OBJ3D& inputObj)
{
	/*initPos = inputObj.initPos;
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

	isHitAble = inputObj.isHitAble;*/

	memcpy(this, &inputObj, sizeof(OBJ3D));
	meshData = inputObj.meshData;
	moveFunc = inputObj.moveFunc;
}

OBJ3D::OBJ3D(const OBJ3D& inputObj)
{
	memberCopy(inputObj);
}

OBJ3D::~OBJ3D()
{
	Clear();
}

const OBJ3D& OBJ3D::operator=(const OBJ3D& rhv)
{
	memberCopy(rhv);
	return *this;
}


void OBJ3D::Clear()
{
	moveFunc = nullptr;
	meshData = nullptr;
	speed = speedAcc = speedMax = size = Vector3(0, 0, 0);
	timer = 0;
	step = 0;
	frame = 0;
	transform.Clear();
	world = Matrix::Identity;

	isInit = false;

	type = 0;
}

void OBJ3D::Update()
{
	if (moveFunc) {
		(this->*moveFunc)();
	}
	//pos.y = initPos.y - SCROLL_Y;
}

void OBJ3D::Draw()
{
	if (meshData)
	{
		meshData->Draw(transform.position, transform.scaling, transform.rotationDegree, frame);
		world = meshData->world;
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