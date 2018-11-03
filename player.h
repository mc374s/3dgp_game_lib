#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "obj3d.h"

#define P_WALK_SPEED (0.02)
#define P_WALK_SPEED_MAX (0.03)
#define P_RUN_SPEED (0.05)
#define P_RUN_SPEED_MAX (0.1)


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
