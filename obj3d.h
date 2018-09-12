#ifndef _OBJ3D_H_
#define _OBJ3D_H_

class OBJ3D
{
protected:
	virtual void memberCopy(const OBJ3D& inputObj);
	OBJ3D();
	OBJ3D(const OBJ3D& inputObj);
	virtual ~OBJ3D();
	const OBJ3D& OBJ3D::operator=(const OBJ3D& rhv);

	MeshData* meshData;
	Vector3 initPos;
	Vector3 setPos;
	Vector3 speed;
	Vector3 speedAcc;
	Vector3 speedMax;

	Vector3 size;
	bool isHitAble;

	Transform transform;


public:
	int timer;
	int step;
	int type;
	bool isInit;
	//void(*m_pfMove)();
	void(OBJ3D::*moveFunc)();


	virtual void clear();
	virtual void update();

	virtual void draw();

	static int searchSet(OBJ3D** begin, int max);


};

#endif // !_OBJ3D_H_
