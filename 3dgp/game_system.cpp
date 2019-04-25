#include "game_system.h"


#include "sprite.h"
#include "render_target.h"
#include "skinned_mesh.h"
#include "primitive3d.h"

#include "system.h"

#include <fstream>

using namespace DirectX;
using namespace GLC;

template<typename Resource, size_t AMOUNT>
void ResourceManager<Resource, AMOUNT>::Load(Resource resources[])
{
	for (int i = 0, max = ARRAYSIZE(resources); i < max && i < AMOUNT; ++i) {
		resources[i].Load();
		pResource[i] = &resources;
	}
}

template<typename Resource, size_t AMOUNT>
const Resource* ResourceManager<Resource,AMOUNT>::At(int resourceNO)
{
	if (resourceNO >= 0 && resourceNO < AMOUNT)
	{
		return pResource[resourceNO];
	}
	else
	{
		return nullptr;
	}
}

template<typename Resource, size_t AMOUNT>
void ResourceManager<Resource, AMOUNT>::Release()
{
	int i;
	for (i = 0; i < AMOUNT; i++)
	{
		if (pResource[i])
		{
			delete pResource[i];
			ZeroMemory(pResource[i], sizeof(*pResource[i]));
		}
	}
	ZeroMemory(pResource, sizeof(Resource)*i);
}


int BasicInput(int controllerNO)
{
	int keyCode = 0x0;

	// @controllerNO check
	if (controllerNO >= Input::MAX_PLAYER_COUNT || controllerNO < 0) {
		_RPT0(_CRT_ERROR, "Illegal controllerNO");
		return 0x0;
	}

	// Assign keyboard to Controller 0(player 1)
	if (controllerNO == 0) {
		if (Input::KEY.Up) {
			keyCode |= PAD_UP;
		}
		if (Input::KEY.Down) {
			keyCode |= PAD_DOWN;
		}
		if (Input::KEY.Left) {
			keyCode |= PAD_LEFT;
		}
		if (Input::KEY.Right) {
			keyCode |= PAD_RIGHT;
		}
		if (Input::KEY_TRACKER.pressed.Enter) {
			keyCode |= PAD_START;
		}
		if (Input::KEY_TRACKER.pressed.Back) {
			keyCode |= PAD_BACK;
		}
		if (Input::KEY_TRACKER.pressed.Z) {
			keyCode |= PAD_TRG1;
		}
		if (Input::KEY_TRACKER.pressed.X) {
			keyCode |= PAD_TRG2;
		}
		if (Input::KEY_TRACKER.pressed.C) {
			keyCode |= PAD_TRG3;
		}
		if (Input::KEY_TRACKER.pressed.V) {
			keyCode |= PAD_TRG4;
		}
	}
	// Assign controller NO to target @controllerNO
	if (Input::PAD[controllerNO].IsConnected()) {
		if (Input::PAD[controllerNO].IsLeftThumbStickUp()) {
			keyCode |= PAD_UP;
		}
		if (Input::PAD[controllerNO].IsLeftThumbStickDown()) {
			keyCode |= PAD_DOWN;
		}
		if (Input::PAD[controllerNO].IsLeftThumbStickLeft()) {
			keyCode |= PAD_LEFT;
		}
		if (Input::PAD[controllerNO].IsLeftThumbStickRight()) {
			keyCode |= PAD_RIGHT;
		}
		if (Input::PAD[controllerNO].IsStartPressed()) {
			keyCode |= PAD_START;
		}
		if (Input::PAD[controllerNO].IsBackPressed()) {
			keyCode |= PAD_BACK;
		}
		if (Input::PAD[controllerNO].IsAPressed()) {
			keyCode |= PAD_TRG1;
		}
		if (Input::PAD[controllerNO].IsBPressed()) {
			keyCode |= PAD_TRG2;
		}
		if (Input::PAD[controllerNO].IsXPressed()) {
			keyCode |= PAD_TRG3;
		}
		if (Input::PAD[controllerNO].IsYPressed()) {
			keyCode |= PAD_TRG4;
		}

	}

	return keyCode;
}

void DrawString(int x, int y, char *buf, UINTCOLOR color, int format, int sizeX, int sizeY, float angle)
{
	SpriteString::DrawString(x, y, buf, XMConvertUIntToColor(color), format, sizeX, sizeY, angle);
}


///////////////////////////////////////////////////////////////
// Class View
View::View(int viewWidth, int viewHeight) :
	doReflection(false),
	position(g_XMZero),
	scale(g_XMOne),
	rotation(g_XMZero)
{
	pRenderTarget = new RenderTarget(System::pd3dDevice, viewWidth, viewHeight);
}

View::View(float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, bool doReflection):
	drawX(drawX), drawY(drawY), drawWidth(drawWidth), drawHeight(drawHeight), 
	srcX(srcX), srcY(srcY), srcWidth(srcWidth), srcHeight(srcHeight),
	rotateAngle(rotateAngle),
	blendColor(blendColor),
	doReflection(doReflection),
	position(g_XMZero),
	scale(g_XMOne),
	rotation(g_XMZero)
{
	pRenderTarget = new RenderTarget(System::pd3dDevice, drawWidth, drawHeight);
}

View::~View()
{
	if (pRenderTarget)
	{
		delete pRenderTarget;
		pRenderTarget = NULL;
	}
}

void View::Set(float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, bool doReflection)
{
	rotation = { 0,0,rotateAngle };
	rotation = XMQuaternionRotationRollPitchYawFromVector(rotation*0.01745f);


	XMMATRIX world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scale, g_XMZero, rotation, position);

	pRenderTarget->Draw(System::pImmediateContext, world, GLC::mainCamera.GetView(), GLC::mainCamera.GetProjection(), drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, XMConvertUIntToColor(blendColor), doReflection);
}

void View::Set()
{
	XMVECTOR rotate = XMQuaternionRotationRollPitchYawFromVector(rotation*0.01745f);

	XMMATRIX world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scale, g_XMZero, rotate, position);

	pRenderTarget->Draw(System::pImmediateContext, world, GLC::mainCamera.GetView(), GLC::mainCamera.GetProjection(), drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, XMConvertUIntToColor(blendColor), doReflection);
}

void View::Clear()
{
	System::pImmediateContext->OMSetRenderTargets(1, &System::pRenderTargetView, System::pDepthStencilView);

	GLC::mainCamera.viewPort.Width = SCREEN_WIDTH;
	GLC::mainCamera.viewPort.Height = SCREEN_HEIGHT;
	System::pImmediateContext->RSSetViewports(1, &GLC::mainCamera.viewPort);
}

///////////////////////////////////////////////////////////

// Class Cube
Cube::Cube(FXMVECTOR position, FXMVECTOR size, const UINTCOLOR &blendColor) :position(position), size(size), blendColor(blendColor)
{
	pPrimitive = new Primitive3D(System::pd3dDevice);
	pPrimitive->Initialize(System::pd3dDevice, GEOMETRY_CUBE);
}

Cube::~Cube()
{
	if (pPrimitive)
	{
		delete pPrimitive;
		pPrimitive = NULL;
	}
}

void Cube::Draw()
{
	//pPrimitive->drawCube(framework::s_pDeviceContext, position, size, blendColor, transform);
}

////////////////////////////////////////////////////////////

// Skinned Mesh Data Management
void MeshData::SetMotion(int fbxFileNO)
{
	if (fbxFileNO >= 0 && fbxFileNO < MAX_MESH_FILE_NUM && pMeshManager->MeshAt(fbxFileNO) && pMeshManager->MeshAt(fbxFileNO)->data) {
		pMeshManager->MeshAt(fileNO)->data->SetAnimation(pMeshManager->MeshAt(fbxFileNO)->data);
	}
}

void XM_CALLCONV MeshData::Draw(FXMVECTOR position, FXMVECTOR scale, FXMVECTOR rotation, int* frame)
{
	if (fileNO >= 0 && fileNO < MAX_MESH_FILE_NUM && pMeshManager->MeshAt(fileNO) && pMeshManager->MeshAt(fileNO)->data) {

		XMVECTOR translation	= position + positionAdjustion;
		XMVECTOR scaleMul		= scale * scaleAdjustion;
		XMVECTOR rotateAdd		= (rotation + rotationAdjustion)*0.01745f;

		XMVECTOR rotation		= XMQuaternionRotationRollPitchYawFromVector(rotateAdd);

		//XMMATRIX worldXMatrix;
		world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scaleMul, g_XMZero, rotation, translation);
		//XMStoreFloat4x4(&world, worldXMatrix);
		pMeshManager->MeshAt(fileNO)->data->Draw(System::pImmediateContext, world, GLC::mainCamera.GetView(), GLC::mainCamera.GetProjection(),
			false, frame/*, Framework::deltaTime*/);
	}
}

void MeshManager::LoadMesh(MeshFile data[], int fileNO)
{
	if (data[fileNO].fileNO != -1 || data[fileNO].path != NULL)
	{
		meshes[fileNO] = new MeshFile(data[fileNO].fileNO, data[fileNO].path);
		meshes[fileNO]->data = new SkinnedMesh(System::pd3dDevice, data[fileNO].path);
	}
}

void MeshManager::LoadMeshes(MeshFile data[])
{
	for (int i = data[0].fileNO; data[i].fileNO != -1 || data[i].path != NULL; i++)
	{
		meshes[i] = new MeshFile(data[i].fileNO, data[i].path);
		meshes[i]->data = new SkinnedMesh(System::pd3dDevice, data[i].path);
	}
}

void MeshManager::LoadMeshes(MeshFile data[], int *progress)
{
	*progress = 0;
	//LARGE_INTEGER meshesSize;
	//meshesSize.QuadPart = 0;
	//LARGE_INTEGER loadedMeshSize;
	//loadedMeshSize.QuadPart = 0;
	//LARGE_INTEGER fileSize;
	//fileSize.QuadPart = 0;
	int meshesSize = 0;
	int loadedMeshSize = 0;
	int fileSize = 0;
	
	for (int i = data[0].fileNO; data[i].fileNO != -1 || data[i].path != NULL; i++)
	{
		std::fstream file(data[i].path, std::ios::in);
		file.seekg(0, std::ios::end);
		meshesSize += file.tellg();
		//GetFileSizeEx(data[i].path, &fileSize);
		//meshesSize.QuadPart += fileSize.QuadPart;

	}

	for (int i = data[0].fileNO; data[i].fileNO != -1 || data[i].path != NULL; i++)
	{
		meshes[i] = new MeshFile(data[i].fileNO, data[i].path);
		meshes[i]->data = new SkinnedMesh(System::pd3dDevice, data[i].path);
		std::fstream file(data[i].path, std::ios::in);
		file.seekg(0, std::ios::end);
		loadedMeshSize+= file.tellg();
		*progress = loadedMeshSize / (float)meshesSize * 100;
		/*GetFileSizeEx(data[i].path, &fileSize);
		loadedMeshSize.QuadPart += fileSize.QuadPart;
		*progress = loadedMeshSize.QuadPart / meshesSize.QuadPart * 100;*/
	}
}

const MeshFile* MeshManager::MeshAt(int fileNO)
{
	if (fileNO >= 0 && fileNO < MAX_MESH_FILE_NUM)
	{
		return meshes[fileNO];
	}
	else
	{
		return nullptr;
	}
}

void MeshManager::ReleaseMeshes()
{
	for (int i = 0; i < MAX_MESH_FILE_NUM; i++)
	{
		if (meshes[i])
		{
			delete meshes[i]->data;
			delete meshes[i];
			//ZeroMemory(meshes[i], sizeof(*meshes[i]));
		}
	}
	ZeroMemory(meshes, sizeof(MeshFile)*MAX_MESH_FILE_NUM);
}

////////////////////////////////////////////////////////////