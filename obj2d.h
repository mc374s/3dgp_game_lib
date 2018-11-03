#ifndef _OBJ2D_H_
#define _OBJ2D_H_

#define OBJ_MAX_NUM	(16)

class OBJ2D
{
private:

public:
	virtual void memberCopy(const OBJ2D& inputObj);
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
	bool isHitAble;

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

	virtual void clear();
	virtual void update();

	virtual void draw();

	static int searchSet(OBJ2D** ppBegin, int max);


};


class OBJ2DEX : public OBJ2D
{
private:

public:

	int aframe;
	int animeNO;
	int animeCounter;

	SPRITE_DATA* pAnimeData;

	virtual void memberCopy(const OBJ2DEX& inputObj);
	OBJ2DEX();
	OBJ2DEX(const OBJ2DEX& inputObj);
	virtual ~OBJ2DEX();
	const OBJ2DEX& operator=(const OBJ2DEX& right);


	virtual void clear();

	virtual void update();
	virtual void animation();

	virtual void draw();

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
	void init();
	void update(int liveInPagination = 1);
	void draw(int liveInPagination = 1);

	ObjManager() {};
	~ObjManager();

};

#define pObjManager (ObjManager::getInstance())

//#define GET_IDLE_OBJ_NO (OBJ2D::searchSet(pObjManager->ppObjs, OBJ_MAX_NUM))


#endif // !_OBJ2D_H_
