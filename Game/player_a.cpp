#include "player.h"

#include "mesh_data.h"
#include "../3dgp/directxtk.h"

PlayerA::PlayerA()
{
	Clear();
}

PlayerA::~PlayerA() 
{
}

void PlayerA::Init()
{
	Player::Init();
	meshData = &fbxPlayerStandby;
	//meshData->SetMotion(FBX_FILE_NO::PLAYER_STANDBY);
}

void PlayerA::Update()
{
	keyCode = BasicInput(controllerNO);
	Player::Update();
}

void PlayerA::Standby()
{
	switch (step) {
	case STEP::INIT:
		frame = 0;
		meshData = &fbxPlayerStandby;
		//meshData->SetMotion(FBX_FILE_NO::PLAYER_STANDBY);
		speed = Vector3(0, 0, 0);
		step = STEP::BEGIN;
		break;
	case STEP::BEGIN:
		if (frame < 0) {
			frame = 0;
		}
		break;
	default:
		break;
	}
}

void PlayerA::Run()
{
	static Vector3 targetRotation(0, 0, 0), startRotation(0, 0, 0);
	static float timeStep = 0, totalTime = 0.2f, elapsedTime = 0;
	static int oldKeyCode = 0x0;
	switch (step) {
	case STEP::INIT:
		moveFunc = &Player::Run;
		meshData = &fbxPlayerRun;
		//meshData->SetMotion(FBX_FILE_NO::PLAYER_RUN);
		frame = 0;
		step = STEP::BEGIN;
		break;
	case STEP::BEGIN:
		if (frame < 0) {
			frame = 0;
		}
		switch (keyCode & (PAD_LEFT | PAD_RIGHT))
		{
		case PAD_LEFT:
			speed.x -= speedAcc.x;
			targetRotation = Vector3(0, 180, 0);
			transform.rotationDegree.y = 180;
			break;
		case PAD_RIGHT:
			speed.x += speedAcc.x;
			targetRotation = Vector3::Zero;
			transform.rotationDegree.y = 0;
			break;
		default:
			if (speed.x > 0) {
				speed.x -= speedAcc.x / 2;
				if (speed.x < 0) {
					speed.x = 0;
				}
			}
			if (speed.x < 0) {
				speed.x += speedAcc.x / 2;
				if (speed.x > 0) {
					speed.x = 0;
				}
			}
			if (fabsf(speed.x - 0.0f) < FLT_EPSILON && timeStep >= 1) {
				speed.x = 0;
				step = STEP::FINISH;
			}
			break;
		}
		if (keyCode && oldKeyCode != keyCode) {
			startRotation = transform.rotationDegree;
			//totalTime= transform.rotationDegree.y/180*2.5f;
			elapsedTime = 0;
			timeStep = 0;
		}
		if (timeStep < 1) {
			timeStep = timeStep > totalTime ? 1 : (elapsedTime += Framework::frameTime) / totalTime;
			transform.rotationDegree = Vector3::Lerp(startRotation, targetRotation, timeStep);
		}
		oldKeyCode = keyCode;

		if (speed.x > speedMax.x) {
			speed.x = speedMax.x;
		}
		if (speed.x < -speedMax.x) {
			speed.x = -speedMax.x;
		}
		break;
	case STEP::FINISH:
		moveFunc = &Player::Standby;
		step = STEP::INIT;
		break;
	default:
		break;
	}
}

void PlayerA::Jump()
{
	switch (step) {
	case STEP::INIT:
		moveFunc = &Player::Jump;
		meshData = &fbxPlayerJump;
		//meshData->SetMotion(FBX_FILE_NO::PLAYER_JUMP);
		frame = 0;
		step = STEP::BEGIN;
		break;
	case STEP::BEGIN:
		if (frame < 0) {
			step = STEP::FINISH;
		}
		++frame;
		// PreMotion, Stop Forward movement
		if (frame < 24) {
			transform.position.x -= speed.x;
		}
		if (frame == 24) {
			speed.y += P_JUMP_V0;
		}
		// Jumping
		if (frame > 36 && speed.y > 0) {
			frame = 36;
		}
		// Dropping
		if (frame > 60 && speed.y < 0) {
			frame = 60;
		}
		// Ending Jump, Also stop forward movement
		if (frame > 72) {
			transform.position.x -= speed.x;
		}
		if (frame > 83) {
			step = STEP::FINISH;
		}
		break;
	case STEP::FINISH:
		moveFunc = &Player::Standby;
		step = STEP::INIT;
		break;
	default:
		break;
	}
}

void PlayerA::Attack()
{
	switch (step) {
	case STEP::INIT:
		moveFunc = &Player::Attack;
		meshData = &fbxPlayerAttack;
		//meshData->SetMotion(FBX_FILE_NO::PLAYER_ATTACK);
		frame = 0;
		step = STEP::BEGIN;
		break;
	case STEP::BEGIN:
		if (frame < 0) {
			step = STEP::FINISH;
		}
		//++frame;

		break;
	case STEP::FINISH:
		moveFunc = &Player::Standby;
		step = STEP::INIT;
		break;
	default:
		break;
	}
}