#include "stage.h"

#include "obj3d.h"
#include "mesh_data.h"
#include "game.h"

#include "../3dgp/directxtk.h"

using namespace GLC;
using namespace Game;

//Stage Game::Stage::instance;

struct StageData {
	MeshData*	mesh;
	Vector3		pos;
	Collision*	collision;
	StageData(MeshData* mesh = nullptr, Vector3 pos = Vector3::Zero, Collision* collision = nullptr) :mesh(mesh), pos(pos) {
		if (collision)
		{
			switch (collision->type)
			{
			case Collision::_SPHERE:
				//(_mm_malloc(sizeof(Sphere), alignof(Sphere)))
				this->collision = new (_mm_malloc(sizeof(Sphere), alignof(Sphere))) Sphere(((Sphere*)collision)->center, ((Sphere*)collision)->radius);
				break;
			case Collision::_AABB:
				this->collision = new (_mm_malloc(sizeof(AABB), alignof(AABB))) AABB(((AABB*)collision)->minPos, ((AABB*)collision)->maxPos);
				break;
			default:
				break;
			}
		}
	}
	~StageData() {
		if (collision) _mm_free(collision);
	}
};


StageData stageData[] = {
	StageData(nullptr,	Vector3(0,0,0),		&AABB(Vector3(-20,-0.2f,-20), Vector3(20,0,20))),
	StageData(nullptr,	Vector3(5,1,0),		&AABB(Vector3(2.5f,1,-1), Vector3(7.5f,1.2f,1))),
	StageData(nullptr,	Vector3(-5,2,0),	&AABB(Vector3(-7.5f,2,-1), Vector3(-2.5f,2.2f,1))),
	StageData(nullptr,	Vector3(10,2,0),	&AABB(Vector3(7.5,2,-1), Vector3(12.5f,2.2f,1))),
	StageData(nullptr,	Vector3(-2,3,0),	&Sphere(Vector3(-2,3,0), 0.5f)),
};

Stage::Stage()
{

}

Stage::~Stage()
{

}

void Stage::Initialize()
{
	OBJ3D temp;
	for (int i = 0, max = sizeof(stageData)/sizeof(StageData); i < max; ++i)
	{
		temp.Clear();
		temp.meshData = stageData[i].mesh;
		temp.transform.position = stageData[i].pos;
		pObjList.push_back(temp);
		pCollisionList.push_back(stageData[i].collision);
	}
}

void Stage::Update()
{

}

void Stage::Draw()
{

	const DirectX::XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	const DirectX::XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
	DXTK::DrawGrid(DirectX::XMMatrixIdentity(), GLC::mainCamera.GetView(), GLC::mainCamera.GetProjection(),
		xaxis, yaxis, DirectX::g_XMZero, 20, 20, DirectX::Colors::Gray);
	for (auto &it : pObjList)
	{
		it.Draw();
	}
	for (auto &it : pCollisionList)
	{
		switch (it->type)
		{
		case Collision::_SPHERE:
			DXTK::DrawSphere(Matrix::Identity, GLC::mainCamera.GetView(), GLC::mainCamera.GetProjection(),
				((Sphere*)it)->center, ((Sphere*)it)->radius, DirectX::Colors::Blue);
			break;
		case Collision::_AABB:
			DXTK::DrawAABB(Matrix::Identity, GLC::mainCamera.GetView(), GLC::mainCamera.GetProjection(),
				((AABB*)it)->minPos, ((AABB*)it)->maxPos, DirectX::Colors::Blue);
			break;
		default:
			break;
		}
	}
}