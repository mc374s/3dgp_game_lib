#include "game.h"
#include "player.h"

#include "mesh_data.h"

void Player::init()
{
	clear();
	speedAcc.z = P_WALK_SPEED;
	speedMax.z = P_WALK_SPEED_MAX;
	meshData = &e_fbxPlayerWalk;
}

void Player::update()
{
	keyCode = basicInput();

	switch (keyCode & (PAD_LEFT | PAD_RIGHT))
	{
	case PAD_LEFT:
		speed.z -= speedAcc.z;
		transform.eulerAngle.x = 180;
		break;
	case PAD_RIGHT:
		speed.z += speedAcc.z;
		transform.eulerAngle.x = 0;
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
	

	transform.position.y += (-15);
	if (transform.position.y < 0)
	{
		transform.position.y = 0;
	}

	if (fabsf(speed.z - 0.0f) > FLT_EPSILON)
	{
		meshData = &e_fbxPlayerWalk;
	}
	if (keyCode & PAD_TRG1)
	{
		meshData = &e_fbxPlayerJump;
		//transform.position.y += 1000;
	}

	transform.position.z += speed.z;
}

void Player::draw()
{
	OBJ3D::draw();
#ifdef DEBUG
	char buf[256];
	sprintf_s(buf, "Player:\nPosX:%lf \nPosY:%lf \nPosZ:%lf \n",
		transform.position.x, transform.position.y, transform.position.z);
	drawString(0, 300, buf);

#endif // DEBUG

}