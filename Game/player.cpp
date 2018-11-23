#include "player.h"

#include "mesh_data.h"
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

	//size = AABB(Vector3(0, 1.64f*0.5f, 0), Vector3(0.4f, 1.64f, 0.4f));
	//size.center = Vector3(0, 1.64f*0.5f, 0);
	//size.size = Vector3(0.4f, 1.64f, 0.4f);
	size.minPos = Vector3(-0.2f, 0, -0.2f);
	size.maxPos = Vector3(0.2f, 1.64f, 0.2f);
	collision = size;

}

void Player::Update()
{
	keyCode = BasicInput();

	oldPos = transform.position;

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
		// PreMotion, Stop Forward movement
		if (frame < 24)
		{
			transform.position.x -= speed.x;
		}
		if (frame == 24)
		{
			speed.y += P_JUMP_V0;
		}
		// Jumping
		if (frame > 36 && speed.y > 0)
		{
			frame = 36;
		}
		// Dropping
		if (frame > 60 && speed.y < 0)
		{
			frame = 60;
		}
		// Ending Jump, Also stop forward movement
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

	if (transform.position.y < RESET_HEIGHT)
	{
		transform.position = Vector3::Zero;
		speed.y = 0;
	}

	static float cameraY = 2, cameraZ = -9;
	if (Input::KEY.I)
	{
		cameraY += 0.01f;
	}
	if (Input::KEY.K)
	{
		cameraY -= 0.01f;
	}
	if (Input::KEY.O)
	{
		cameraZ += 0.01f;
	}
	if (Input::KEY.U)
	{
		cameraZ -= 0.01f;
	}
	
	GLC::mainCamera.eyePosition = transform.position + Vector3(0, cameraY, cameraZ);
	GLC::mainCamera.focusPosition = transform.position + Vector3(0, 0.5f, 0);

	Vector3 forword = DirectX::XMVectorSubtract(GLC::mainCamera.focusPosition, GLC::mainCamera.eyePosition);
	forword.Normalize();
	
	float angle = forword.Dot(Vector3(0, 1, 0));
	GLC::mainCamera.upDirection = Vector3(0, 1, 0) - forword*angle;

	GLC::mainCamera.upDirection = DirectX::XMVector3Normalize(GLC::mainCamera.upDirection);

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

	/*Vector3 positionAdd = meshData->preSetTransform.position + transform.position;
	Vector3 rotateAdd = (transform.eulerDegreeAngle)*0.01745f;

	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(rotateAdd.x, rotateAdd.y, rotateAdd.z);

	static XMMATRIX world;
	world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), g_XMOne, g_XMZero, rotate, XMLoadFloat3(&transform.position));*/

	//static AABB testAABB(Vector3(5.0f, 0.0f, -0.2f), Vector3(6.0f, 1.0f, 0.2f));
	//DXTK::DrawAABB(Framework::pDeviceContext, Matrix::Identity, GLC::mainCamera.view, GLC::mainCamera.projection,
	//	testAABB.minPos, testAABB.maxPos, DirectX::Colors::Green);
	//static Sphere testSphere(Vector3(-5.0f, 0.0f, -0.2f), 1);
	//DXTK::DrawSphere(Framework::pDeviceContext, Matrix::Identity, GLC::mainCamera.view, GLC::mainCamera.projection,
	//	testSphere.center, testSphere.radius,DirectX::Colors::Magenta);

	////collision.minPos = DirectX::XMVector3Transform(size.minPos, world);
	////collision.maxPos = DirectX::XMVector3Transform(size.maxPos, world);
	//DirectX::XMVECTOR color = DirectX::Colors::Green;
	//if (collision.HitJudgement(&testAABB) || collision.HitJudgement(&testSphere))
	//{
	//	color = DirectX::Colors::Red;
	//}
	DXTK::DrawAABB(Framework::pDeviceContext, Matrix::Identity, GLC::mainCamera.view, GLC::mainCamera.projection,
		collision.minPos, collision.maxPos, collisionColor);


#ifdef DEBUG
	char buf[256];
	sprintf_s(buf, "Player:\nPosX:%lf \nPosY:%lf \nPosZ:%lf \n SpeedX:%lf \nSpeedY:%lf \nSpeedZ:%lf \n",
		transform.position.x, transform.position.y, transform.position.z, speed.x, speed.y, speed.z);
	DrawString(0, 300, buf, 0xFFFFFFFF, STR_LEFT, 16, 16);

#endif // DEBUG

}


PlayerManager::PlayerManager()
{}

PlayerManager::~PlayerManager()
{
	//SAFE_DELETE(pPlayer);
	_mm_free(pPlayer);
}

void PlayerManager::Init()
{
	if (!pPlayer)
	{
		//pPlayer = new Player();
		
		// Heap memory Alignment
		pPlayer = new (_mm_malloc(sizeof(Player), alignof(Player))) Player();
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

void PlayerManager::DetectCollision(Collision* other)
{
	HitResult hitResult = pPlayer->collision.HitJudgement(other);
	// 衝突したらプレイヤーを判定相手から外へ押し出し
	if (hitResult.isHitted)
	{
		pPlayer->collisionColor= DirectX::Colors::Red;
		//static Vector3 size = Vector3(pPlayer->size.maxPos) - Vector3(pPlayer->size.minPos);
		//static float disFromCenterToVertex = size.x*size.x + size.y*size.y;
		Vector2 direction = Vector2(Vector3(pPlayer->size.maxPos) - Vector3(pPlayer->size.minPos));
		direction.Normalize();
		static float dotRangeUpAndDown = -fabsf(Vector2::UnitY.Dot(direction));
		static float dotRangeLeftAndRight = -fabsf(Vector2::UnitX.Dot(direction));
		
		if (Vector3::Left.Dot(hitResult.direction) < dotRangeLeftAndRight && Vector3::Left.Dot(pPlayer->moveDirection) < 0)
		{
			pPlayer->speed.x = 0;
			pPlayer->transform.position.x = Vector3(hitResult.closestPoint).x - Vector3(pPlayer->size.maxPos).x/* - 0.001f*/;
		}
		else if (Vector3::Right.Dot(hitResult.direction) < dotRangeLeftAndRight && Vector3::Right.Dot(pPlayer->moveDirection) < 0)
		{
			pPlayer->speed.x = 0;
			pPlayer->transform.position.x = Vector3(hitResult.closestPoint).x + Vector3(pPlayer->size.maxPos).x/* + 0.001f*/;
		}
		else if (Vector3::Up.Dot(hitResult.direction) < dotRangeUpAndDown && Vector3::Up.Dot(pPlayer->moveDirection) < 0)
		{
			pPlayer->speed.y = 0;
			pPlayer->transform.position.y = Vector3(hitResult.closestPoint).y/* + 0.001f*/;
		}
		else if (Vector3::Down.Dot(hitResult.direction) < dotRangeUpAndDown && Vector3::Down.Dot(pPlayer->moveDirection) < 0)
		{
			pPlayer->speed.y = 0;
			pPlayer->transform.position.y = Vector3(hitResult.closestPoint).y - Vector3(DirectX::XMVectorSubtract(pPlayer->size.maxPos, pPlayer->size.minPos)).y/* - 0.001f*/;
		}
	}
}