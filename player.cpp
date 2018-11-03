#include "game.h"
#include "player.h"

#include "mesh_data.h"

Player::Player()
{
	clear();
}

Player::~Player()
{
}

void Player::init()
{
	clear();
	speedAcc.x = P_WALK_SPEED;
	speedMax.x = P_WALK_SPEED_MAX;
	meshData = &e_fbxPlayerWalk;
}

void Player::update()
{
	keyCode = basicInput();

	switch (keyCode & (PAD_LEFT | PAD_RIGHT))
	{
	case PAD_LEFT:
		speed.x -= speedAcc.x;
		transform.eulerAngle.y = 180;
		break;
	case PAD_RIGHT:
		speed.x += speedAcc.x;
		transform.eulerAngle.y = 0;
		break;
	default:
		if (speed.x > 0) {
			speed.x -= speedAcc.x / 2;
			if (speed.x < 0) speed.x = 0;
		}
		if (speed.x < 0) {
			speed.x += speedAcc.x / 2;
			if (speed.x > 0) speed.x = 0;
		}
		break;
	}


	if (speed.x > speedMax.x)
	{
		speed.x = speedMax.x;
	}
	if (speed.x < -speedMax.x)
	{
		speed.x = -speedMax.x;
	}
	

	transform.position.y += (-1);
	if (transform.position.y < 0)
	{
		transform.position.y = 0;
	}

	if (fabsf(speed.x - 0.0f) > FLT_EPSILON)
	{
		meshData = &e_fbxPlayerWalk;
	}
	if (keyCode & PAD_TRG1)
	{
		//meshData = &e_fbxPlayerJump;z
		//transform.position.y += 1000;
	}
	

	transform.position.x += speed.x;


	e_mainCamera.upDirection = { 0,1,0, 0 };
	e_mainCamera.eyePosition = { transform.position.x, transform.position.y + 0.5f,transform.position.z - 3.0f,0 };
	e_mainCamera.focusPosition = { transform.position.x, transform.position.y + 0.5f, transform.position.z,0 };

	//e_mainCamera.toNDC();
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


PlayerManager::PlayerManager()
{}

PlayerManager::~PlayerManager()
{
	SAFE_DELETE(pPlayer);
}

void PlayerManager::init()
{
	if (!pPlayer)
	{
		pPlayer = new Player();
		pPlayer->init();
	}
	else
	{
		pPlayer->init();
	}
}

void PlayerManager::update()
{
	if (pPlayer)
	{
		pPlayer->update();
	}
}

void PlayerManager::draw()
{
	if (pPlayer)
	{
		pPlayer->draw();
	}
}