#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "obj3d.h"

#define P_WALK_SPEED (0.02f)
#define P_WALK_SPEED_MAX (0.03f)
#define P_RUN_SPEED (0.05f)
#define P_RUN_SPEED_MAX (0.1f)
#define P_GS (9.8ff)
#define P_GF (0.0027f) //P_GS/3600.0f
#define P_JUMP_HEIGHT (1.5f)
//#define P_JUMP_V0 (9.9f/60.0f) //sqrt(2*P_GF*(5.0f))
#define P_JUMP_V0 (sqrt(2.0f*P_GF*P_JUMP_HEIGHT))


/*__declspec(align(16)) */class Player : public OBJ3D
{
private:


public:

	enum TYPE {
		A=0,
		B,
		MAX_NUM
	};

	int controllerNO;
	int keyCode;
	GLC::AABB size;
	GLC::AABB collision;
	Vector4 collisionColor;
	Vector3 oldPos;
	Vector3 moveDirection;

	Player();
	virtual ~Player();

	virtual void Clear();
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	//void(Player*)();
	void(Player::*moveFunc)();
	void(Player::*lastMove)();

	virtual void Standby() {};
	virtual void Run() {};
	virtual void Jump() {};
	virtual void Attack() {};

};

class PlayerA:public Player
{
private:


public:
	PlayerA();
	virtual ~PlayerA();

	virtual void Initialize();
	virtual void Update();

	virtual void Standby();
	virtual void Run();
	virtual void Jump();
	virtual void Attack();
};

class PlayerB :public Player
{
private:


public:
	PlayerB();
	virtual ~PlayerB();

	virtual void Initialize();
	virtual void Update();

	virtual void Standby();
	virtual void Run();
	virtual void Jump();
	virtual void Attack();
};




class PlayerManager :public Singleton<PlayerManager> {

private:

	Player* pPlayer[Input::MAX_PLAYER_COUNT] = { nullptr };

public:
	PlayerManager();
	~PlayerManager();

	int playerNum;

	void Initialize(int playerNum = 1, int playerType[] = {0});
	void Update();
	void Draw();

	void DetectCollision(GLC::Collision* other);

};

#define pPlayerManager (PlayerManager::GetInstance())


#endif // !_PLAYER_H_
