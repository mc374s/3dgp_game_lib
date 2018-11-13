#include "game.h"
#include "player.h"

#include "mesh_data.h"
#include "../3dgp/camera.h"

Player::Player()
{
	Clear();
}

Player::~Player()
{
}

void Player::init()
{
	Clear();
	speedAcc.x = P_WALK_SPEED;
	speedMax.x = P_WALK_SPEED_MAX;
	meshData = &e_fbxPlayerWalk;
	
}

void Player::Update()
{
	keyCode = BasicInput();

	switch (keyCode & (PAD_LEFT | PAD_RIGHT))
	{
	case PAD_LEFT:
		speed.x -= speedAcc.x;
		transform.eulerDegreeAngle.y = 180;
		break;
	case PAD_RIGHT:
		speed.x += speedAcc.x;
		transform.eulerDegreeAngle.y = 0;
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

	/*if (fabsf(speed.x - 0.0f) > FLT_EPSILON)
	{
		frame = 0;
		meshData = &e_fbxPlayerWalk;
	}*/
	transform.position.x += speed.x;

	if (keyCode & PAD_TRG1)
	{
		meshData = &e_fbxPlayerJump;
		++frame;
	}
	if (frame > 0)
	{
		++frame;
		if (frame < 24)
		{
			transform.position.x -= speed.x;
		}
		if (frame == 24)
		{
			speed.y += P_JUMP_V0;
		}
		if (frame > 36 && speed.y > 0)
		{
			frame = 36;
		}
		if (frame > 60 && speed.y < 0)
		{
			frame = 60;
		}
		if (frame > 72)
		{
			transform.position.x -= speed.x;
		}
		if (frame > 83)
		{
			frame = 0;
			meshData = &e_fbxPlayerWalk;
		}

	}
	speed.y -= P_GF;
	transform.position.y += speed.y;

	if (transform.position.y < 0)
	{
		transform.position.y = 0;
		speed.y = 0;
	}

	static float cameraY = 2, cameraZ = -9;
	if (KEY_BOARD.I)
	{
		cameraY += 0.01;
	}
	if (KEY_BOARD.K)
	{
		cameraY -= 0.01;
	}
	if (KEY_BOARD.O)
	{
		cameraZ += 0.01;
	}
	if (KEY_BOARD.U)
	{
		cameraZ -= 0.01;
	}
	
	Camera::mainCamera.eyePosition = { transform.position.x, transform.position.y + cameraY,transform.position.z + cameraZ,0 };
	Camera::mainCamera.focusPosition = { transform.position.x, transform.position.y + 0.5f, transform.position.z,0 };

	Vector3 forword = XMVectorSubtract(Camera::mainCamera.focusPosition, Camera::mainCamera.eyePosition);
	forword.Normalize();
	float angle = forword.Dot(Vector3(0, 1, 0));
	Camera::mainCamera.upDirection = Vector3(0, 1, 0) - forword*angle;

	Camera::mainCamera.upDirection = XMVector3Normalize(Camera::mainCamera.upDirection);
	
	//Camera::mainCamera.toNDC();
}

void Player::Draw()
{
	OBJ3D::Draw();
#ifdef DEBUG
	char buf[256];
	sprintf_s(buf, "Player:\nPosX:%lf \nPosY:%lf \nPosZ:%lf \n",
		transform.position.x, transform.position.y, transform.position.z);
	DrawString(0, 300, buf);

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

void PlayerManager::Update()
{
	if (pPlayer)
	{
		pPlayer->Update();
	}
}

void PlayerManager::Draw()
{
	if (pPlayer)
	{
		pPlayer->Draw();
	}
}