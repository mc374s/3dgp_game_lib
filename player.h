#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "obj3d.h"

#define P_WALK_SPEED (0.02)
#define P_WALK_SPEED_MAX (0.03)
#define P_RUN_SPEED (0.05)
#define P_RUN_SPEED_MAX (0.1)
#define P_GS (9.8f)
#define P_GF (0.0027f) //P_GS/3600.0f
#define P_JUMP_HEIGHT (5.0f)
#define P_JUMP_V0 (9.9f/60.0f) //sqrt(2*P_GF*P_JUMP_HEIGHT)


class Player : public OBJ3D
{
private:


public:

	int keyCode;

	Player();
	~Player();

	void init();
	void update();
	void draw();

};



class PlayerManager :public Singleton<PlayerManager> {

private:
	Player* pPlayer = nullptr;

public:
	PlayerManager();
	~PlayerManager();

	void init();
	void update();
	void draw();

};

#define pPlayerManager (PlayerManager::getInstance())


#endif // !_PLAYER_H_
