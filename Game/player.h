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


class Player : public OBJ3D
{
private:


public:

	int keyCode;

	Player();
	~Player();

	void Init();
	void Update();
	void Draw();

};



class PlayerManager :public Singleton<PlayerManager> {

private:
	Player* pPlayer = nullptr;

public:
	PlayerManager();
	~PlayerManager();

	void Init();
	void Update();
	void Draw();

};

#define pPlayerManager (PlayerManager::getInstance())


#endif // !_PLAYER_H_
