#include "game.h"
#include "player.h"

#include "mesh_data.h"
#include "../3dgp/camera.h"
#include "../3dgp/directxtk.h"

Player::Player()
{
	Clear();
}

Player::~Player()
{
}

void Player::Init()
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
		transform.rotationDegree.y = 180;
		break;
	case PAD_RIGHT:
		speed.x += speedAcc.x;
		transform.rotationDegree.y = 0;
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
	if (Input::KEY.I)
	{
		cameraY += 0.01;
	}
	if (Input::KEY.K)
	{
		cameraY -= 0.01;
	}
	if (Input::KEY.O)
	{
		cameraZ += 0.01;
	}
	if (Input::KEY.U)
	{
		cameraZ -= 0.01;
	}
	
	GLC::mainCamera.eyePosition = transform.position + Vector3(0, cameraY, cameraZ);
	GLC::mainCamera.focusPosition = transform.position + Vector3(0, 0.5f, 0);

	Vector3 forword = DirectX::XMVectorSubtract(GLC::mainCamera.focusPosition, GLC::mainCamera.eyePosition);
	forword.Normalize();
	
	float angle = forword.Dot(Vector3(0, 1, 0));
	GLC::mainCamera.upDirection = Vector3(0, 1, 0) - forword*angle;

	GLC::mainCamera.upDirection = DirectX::XMVector3Normalize(GLC::mainCamera.upDirection);


	
}

void Player::Draw()
{
	OBJ3D::Draw();

	/*Vector3 positionAdd = meshData->preSetTransform.position + transform.position;
	Vector3 rotateAdd = (transform.eulerDegreeAngle)*0.01745f;

	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(rotateAdd.x, rotateAdd.y, rotateAdd.z);

	static XMMATRIX world;
	world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), g_XMOne, g_XMZero, rotate, XMLoadFloat3(&transform.position));*/

	DXTK::DrawAABB(Framework::pDeviceContext, meshData->world, GLC::mainCamera.view, GLC::mainCamera.projection, Vector3(-0.5, 0, 0.5), Vector3(0.5, 2, -0.5), DirectX::Colors::Green);

	DXTK::DrawSphere(Framework::pDeviceContext, meshData->world, GLC::mainCamera.view, GLC::mainCamera.projection, DirectX::g_XMZero, 0.5, DirectX::Colors::Red);


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

void PlayerManager::Init()
{
	if (!pPlayer)
	{
		pPlayer = new Player();
		pPlayer->Init();
	}
	else
	{
		pPlayer->Init();
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