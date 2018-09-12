#include "game.h"
#include "player.h"

void Player::update()
{
	keyCode = basicInput();

	switch (keyCode & (PAD_LEFT | PAD_RIGHT))
	{
	case PAD_LEFT:
		speed.z -= speedAcc.z;
		transform.eulerAngle.x = 0;
		break;
	case PAD_RIGHT:
		speed.z += speedAcc.z;
		transform.eulerAngle.x = 180;
		break;
	default:
		if (speed.z > 0) {
			speed.z -= speedAcc.z / 2;
			if (speed.z < 0) speed.z = 0;
		}
		if (speed.z < 0) {
			speed.z += speedAcc.z / 2;
			if (speed.z > 0) speed.z = 0;
		}
		break;
	}


	if (speed.z > speedMax.z)
	{
		speed.z = speedMax.z;
	}
	if (speed.z < -speedMax.z)
	{
		speed.z = -speedMax.z;
	}
	
	if (fabsf(speed.z - 0.0f) > FLT_EPSILON)
	{

	}

	transform.position.z += speed.z;
}