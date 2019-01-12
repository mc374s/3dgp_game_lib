#include "player.h"

#include "mesh_data.h"
#include "../3dgp/directxtk.h"

Player::Player()
{
	
}

Player::~Player()
{
}

void Player::Clear()
{
	OBJ3D::Clear();
	size.minPos = Vector3(-0.2f, 0, -0.2f);
	size.maxPos = Vector3(0.2f, 1.64f, 0.2f);
	collision = size;
}

void Player::Init()
{
	Clear();
	speedAcc.x = P_WALK_SPEED;
	speedMax.x = P_WALK_SPEED_MAX;
	meshData = &fbxPlayerStandby;
	moveFunc = &Player::Standby;

	//size = AABB(Vector3(0, 1.64f*0.5f, 0), Vector3(0.4f, 1.64f, 0.4f));
	//size.center = Vector3(0, 1.64f*0.5f, 0);
	//size.size = Vector3(0.4f, 1.64f, 0.4f);
	size.minPos = Vector3(-0.2f, 0, -0.2f);
	size.maxPos = Vector3(0.2f, 1.64f, 0.2f);
	collision = size;

}

void Player::Update()
{
	oldPos = transform.position;

	if (keyCode & (PAD_LEFT | PAD_RIGHT) && moveFunc == &Player::Standby) {
		step = STEP::INIT;
		lastMove = moveFunc;
		moveFunc = &Player::Run;
	}
	if (keyCode & PAD_TRG1 && moveFunc != &Player::Jump){
		step = STEP::INIT;
		lastMove = moveFunc;
		moveFunc = &Player::Jump;
	}
	if (keyCode & PAD_TRG2 && moveFunc != &Player::Attack) {
		step = STEP::INIT;
		lastMove = moveFunc;
		moveFunc = &Player::Attack;
	}

	if (moveFunc) {
		(this->*moveFunc)();
	}
	

	speed.y -= P_GF;
	transform.position += speed;

	if (transform.position.y < RESET_HEIGHT) {
		transform.position = Vector3::Zero;
		speed.y = 0;
	}

	// Update collision;
	collision.minPos = Vector3(size.minPos) + transform.position;
	collision.maxPos = Vector3(size.maxPos) + transform.position;
	collisionColor = DirectX::Colors::Green;

	moveDirection = transform.position - oldPos;
	moveDirection.Normalize();

}

void Player::Draw()
{
	OBJ3D::Draw();

	DXTK::DrawAABB(Framework::pDeviceContext, Matrix::Identity, GLC::mainCamera.view, GLC::mainCamera.projection,
		collision.minPos, collision.maxPos, collisionColor);


#ifdef DEBUG
	char buf[256];
	sprintf_s(buf, "Player:\nPosX:%lf \nPosY:%lf \nPosZ:%lf \nSpeedX:%lf \nSpeedY:%lf \nSpeedZ:%lf \n",
		transform.position.x, transform.position.y, transform.position.z, speed.x, speed.y, speed.z);
	DrawString(0, 300, buf, 0xFFFFFFFF, STR_LEFT, 16, 16);

#endif // DEBUG

}





PlayerManager::PlayerManager()
{}

PlayerManager::~PlayerManager()
{
	//SAFE_DELETE(pPlayer);
	for (int i = 0; i < Input::MAX_PLAYER_COUNT && i < playerNum; ++i) {
		if (pPlayer[i]) {
			//_mm_free(pPlayer[i]);
			delete pPlayer[i];
		}
	}
}

void PlayerManager::Init(int _playerNum, int playerType[])
{
	if (_playerNum <= Input::MAX_PLAYER_COUNT && _playerNum > 0) {
		playerNum = _playerNum;
	}
	
	for (int i = 0; i < Input::MAX_PLAYER_COUNT && i < playerNum; ++i) {
		if (pPlayer[i]) {
			//_mm_free(pPlayer[i]);
			delete pPlayer[i];
		}
		switch (playerType[i]) {
		case Player::TYPE::A:
			//pPlayer[i] = new (_mm_malloc(sizeof(PlayerA), alignof(PlayerA))) PlayerA();
			pPlayer[i] = new PlayerA();
			break;
		case Player::TYPE::B:
			//pPlayer[i] = new (_mm_malloc(sizeof(PlayerB), alignof(PlayerB))) PlayerB();
			pPlayer[i] = new PlayerB();
			break;
		default:
			break;
		}

		pPlayer[i]->Init();
		pPlayer[i]->controllerNO = i;

		pPlayer[i]->transform.position.x += i * 2;
	}
}

void PlayerManager::Update()
{
	static float cameraY = 2, cameraZ = -9;
	Vector3 centerPositions(0, 0, 0);
	Vector3 minPosition(0, 0, 0), maxPosition(0, 0, 0);

	for (int i = 0; i < playerNum; ++i) {
		if (pPlayer[i]) {
			pPlayer[i]->Update();
			if (pPlayer[i]->transform.position.x < minPosition.x) {
				minPosition.x = pPlayer[i]->transform.position.x;
			}
			if (pPlayer[i]->transform.position.x > maxPosition.x) {
				maxPosition.x = pPlayer[i]->transform.position.x;
			}
		}
	}

	centerPositions = (minPosition + maxPosition) / 2.0f;


	// Camera Work
	if (Input::KEY.I) {
		cameraY += 0.01f;
	}
	if (Input::KEY.K) {
		cameraY -= 0.01f;
	}
	if (Input::KEY.O) {
		cameraZ += 0.01f;
	}
	if (Input::KEY.U) {
		cameraZ -= 0.01f;
	}

	GLC::mainCamera.eyePosition = centerPositions + Vector3(0, cameraY, cameraZ);
	GLC::mainCamera.focusPosition = centerPositions + Vector3(0, 0.5f, 0);

	Vector3 forword = DirectX::XMVectorSubtract(GLC::mainCamera.focusPosition, GLC::mainCamera.eyePosition);
	forword.Normalize();

	float angle = forword.Dot(Vector3(0, 1, 0));
	GLC::mainCamera.upDirection = Vector3(0, 1, 0) - forword*angle;

	GLC::mainCamera.upDirection = DirectX::XMVector3Normalize(GLC::mainCamera.upDirection);

}

void PlayerManager::Draw()
{
	for (int i = 0; i < playerNum; ++i) {
		if (pPlayer[i]) {
			pPlayer[i]->Draw();
		}
	}
}

void PlayerManager::DetectCollision(Collision* other)
{
	HitResult hitResult;
	for (int i = 0; i < playerNum; ++i) {
		if (pPlayer[i]) {
			hitResult = pPlayer[i]->collision.HitJudgement(other);
			// 衝突したらプレイヤーを判定相手から外へ押し出し
			if (hitResult.isHitted) {
				pPlayer[i]->collisionColor = DirectX::Colors::Red;
				//static Vector3 size = Vector3(pPlayer[i]->size.maxPos) - Vector3(pPlayer[i]->size.minPos);
				//static float disFromCenterToVertex = size.x*size.x + size.y*size.y;
				Vector2 direction = Vector2(Vector3(pPlayer[i]->size.maxPos) - Vector3(pPlayer[i]->size.minPos));
				direction.Normalize();
				static float dotRangeUpAndDown = -fabsf(Vector2::UnitY.Dot(direction));
				static float dotRangeLeftAndRight = -fabsf(Vector2::UnitX.Dot(direction));

				if (Vector3::Left.Dot(hitResult.direction) < dotRangeLeftAndRight && Vector3::Left.Dot(pPlayer[i]->moveDirection) < 0) {
					pPlayer[i]->speed.x = 0;
					pPlayer[i]->transform.position.x = Vector3(hitResult.closestPoint).x - Vector3(pPlayer[i]->size.maxPos).x/* - 0.001f*/;
				}
				else if (Vector3::Right.Dot(hitResult.direction) < dotRangeLeftAndRight && Vector3::Right.Dot(pPlayer[i]->moveDirection) < 0) {
					pPlayer[i]->speed.x = 0;
					pPlayer[i]->transform.position.x = Vector3(hitResult.closestPoint).x + Vector3(pPlayer[i]->size.maxPos).x/* + 0.001f*/;
				}
				else if (Vector3::Up.Dot(hitResult.direction) < dotRangeUpAndDown && Vector3::Up.Dot(pPlayer[i]->moveDirection) < 0) {
					pPlayer[i]->speed.y = 0;
					pPlayer[i]->transform.position.y = Vector3(hitResult.closestPoint).y/* + 0.001f*/;
				}
				else if (Vector3::Down.Dot(hitResult.direction) < dotRangeUpAndDown && Vector3::Down.Dot(pPlayer[i]->moveDirection) < 0) {
					pPlayer[i]->speed.y = 0;
					pPlayer[i]->transform.position.y = Vector3(hitResult.closestPoint).y - Vector3(DirectX::XMVectorSubtract(pPlayer[i]->size.maxPos, pPlayer[i]->size.minPos)).y/* - 0.001f*/;
				}
			}

		}
	}
}