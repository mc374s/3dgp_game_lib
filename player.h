#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "obj3d.h"

#define P_WALK_SPEED (2)
#define P_WALK_SPEED_MAX (16.5)
#define P_RUN_SPEED (1.5)
#define P_RUN_SPEED_MAX (10)


class Player : public OBJ3D
{
private:


public:
	int keyCode;

	void init();
	void update();
	void draw();
};





#endif // !_PLAYER_H_
