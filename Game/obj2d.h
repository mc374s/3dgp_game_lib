#ifndef _OBJ2D_H_
#define _OBJ2D_H_

#include "game.h"

#define OBJ_MAX_NUM	(16)

class OBJ2D
{
private:

public:
	virtual void MemberCopy(const OBJ2D& inputObj);
	OBJ2D();
	OBJ2D(const OBJ2D& inputObj);
	virtual ~OBJ2D();
	const OBJ2D& OBJ2D::operator=(const OBJ2D& right);

	SPRITE_DATA* pSprData;
	Vector3 pos;
	Vector3 initPos;
	Vector3 setPos;
	Vector3 speed;
	Vector3 speedAcc;
	Vector3 speedMax;

	Vector3 size;
	bool isHitable;

	Transform2D transform2D;
	Transform transform;
	//void(*pfMove)();
	void(OBJ2D::*pfMove)();

	int timer;
	int step;
	int alpha;
	int setAlpha;
	int type;

	bool isInit;

	virtual void Clear();
	virtual void Update();

	virtual void Draw();

	static int SearchSet(OBJ2D** ppBegin, int max);


};


class OBJ2DEX : public OBJ2D
{
private:

public:

	int aframe;
	int animeNO;
	int animeCounter;

	SPRITE_DATA* pAnimeData;

	virtual void MemberCopy(const OBJ2DEX& inputObj);
	OBJ2DEX();
	OBJ2DEX(const OBJ2DEX& inputObj);
	virtual ~OBJ2DEX();
	const OBJ2DEX& operator=(const OBJ2DEX& right);


	virtual void Clear();

	virtual void Update();
	virtual void animation();

	virtual void Draw();

};

class Manager
{
public:
	int timer;
	int step;
	Manager() {
		timer = 0;
		step = 0;
	};
	virtual ~Manager() {};

private:

};


class ObjManager : public Singleton<ObjManager>, public Manager
{
private:

public:
	void Initialize();
	void Update(int liveInPagination = 1);
	void Draw(int liveInPagination = 1);

	ObjManager() {};
	~ObjManager();

};

#define pObjManager (ObjManager::GetInstance())

//#define GET_IDLE_OBJ_NO (OBJ2D::searchSet(pObjManager->ppObjs, OBJ_MAX_NUM))


#endif // !_OBJ2D_H_
