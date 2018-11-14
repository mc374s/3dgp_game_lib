#include "game_system.h"

#include "blend.h"
#include "sprite.h"
#include "render_target.h"
#include "skinned_mesh.h"
#include "primitive3d.h"

#include <fstream>


void SPRITE_DATA::Draw(float x, float y,const Transform2D& transform2D) {
	if (texNO >= 0 && texNO < TEX_MAX && pTextureManager->textureAt(texNO) && pTextureManager->textureAt(texNO)->img)
	{
		pTextureManager->textureAt(texNO)->img->Draw(framework::s_pDeviceContext, x + ofsX, y + ofsY, transform2D.scaleX*width, transform2D.scaleY*height, left, top, width, height,
			XMConvertUIntToColor(transform2D.rgba), transform2D.angle, transform2D.centRotate, transform2D.centX, transform2D.centY, transform2D.reflectX, transform2D.scaleMode);
	}
}

void SPRITE_DATA::Draw(Vector3 &pos, const Transform2D& transform2D, const Transform& transform) {
	if (texNO >= 0 && texNO < TEX_MAX && pTextureManager->textureAt(texNO) && pTextureManager->textureAt(texNO)->img) {
		if (pTextureManager->textureAt(texNO)->doProjection)
		{
			static XMMATRIX world, view, projection;
			//world = view = projection = DirectX::XMMatrixIdentity();
			const XMVECTORF32 translate = { transform.position.x, transform.position.y, transform.position.z };
			const XMVECTORF32 scale = { transform.scaling.x, transform.scaling.y, transform.scaling.z };
			XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(transform.eulerDegreeAngle.x, transform.eulerDegreeAngle.y, transform.eulerDegreeAngle.z);

			world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
			view = DirectX::XMMatrixLookAtLH(Camera::mainCamera.eyePosition, Camera::mainCamera.focusPosition, Camera::mainCamera.upDirection);
			projection = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);

			pTextureManager->textureAt(texNO)->img->Draw(framework::s_pDeviceContext, world, view, projection, pos.x + ofsX, pos.y + ofsY, transform2D.scaleX*width, transform2D.scaleY*height, left, top, width, height,
				XMConvertUIntToColor(transform2D.rgba), transform2D.angle, transform2D.centRotate, transform2D.centX, transform2D.centY, transform2D.reflectX, transform2D.scaleMode);
		}
		else
		{
			Draw(pos.x, pos.y, transform2D);
		}
	}
}

void TextureManager::LoadTextures(LOAD_TEXTURE data[])
{
	for (int i = data[0].texNO; data[i].texNO != -1 || data[i].fileName != NULL; i++)
	{
		data[i].img = new Sprite(framework::s_pDevice, data[i].fileName, data[i].doProjection);
		pTextures[i] = &data[i];
	}
}

void TextureManager::LoadTexture(LOAD_TEXTURE data[], int textureNO)
{
	if (data[textureNO].texNO != -1 || data[textureNO].fileName != NULL)
	{
		data[textureNO].img = new Sprite(framework::s_pDevice, data[textureNO].fileName, data[textureNO].doProjection);
		pTextures[textureNO] = &data[textureNO];
	}
}

const LOAD_TEXTURE* TextureManager::textureAt(int fileNO)
{
	if (fileNO >= 0 && fileNO < TEX_MAX)
	{
		return pTextures[fileNO];
	}
	else
	{
		return nullptr;
	}
}

void TextureManager::ReleaseTexture()
{
	int i;
	for (i = 0; i < TEX_MAX; i++)
	{
		if (pTextures[i])
		{
			delete pTextures[i]->img;
			ZeroMemory(pTextures[i], sizeof(*pTextures[i]));
		}
	}
	ZeroMemory(pTextures, sizeof(LOAD_TEXTURE)*i);
}


int  BasicInput()
{
	int command = 0x0;

	if (Input::KEY_BOARD.Up || Input::GAME_PAD.IsLeftThumbStickUp()) {
		command |= PAD_UP;
	}
	if (Input::KEY_BOARD.Down || Input::GAME_PAD.IsLeftThumbStickDown()) {
		command |= PAD_DOWN;
	}
	if (Input::KEY_BOARD.Left || Input::GAME_PAD.IsLeftThumbStickLeft()){
		command |= PAD_LEFT;
	}
	if (Input::KEY_BOARD.Right || Input::GAME_PAD.IsLeftThumbStickRight()){
		command |= PAD_RIGHT;
	}
	if (Input::KEY_TRACKER.pressed.Enter || Input::GAME_PAD.IsStartPressed()) {
		command |= PAD_START;
	}
	if (Input::KEY_TRACKER.pressed.Escape || Input::GAME_PAD.IsBackPressed()) {
		command |= PAD_BACK;
	}
	if (Input::KEY_TRACKER.pressed.Z || Input::GAME_PAD.IsAPressed()) {
		command |= PAD_TRG1;
	}
	if (Input::KEY_TRACKER.pressed.X || Input::GAME_PAD.IsBPressed()) {
		command |= PAD_TRG2;
	}
	if (Input::KEY_TRACKER.pressed.C || Input::GAME_PAD.IsXPressed()) {
		command |= PAD_TRG3;
	}
	if (Input::KEY_TRACKER.pressed.V || Input::GAME_PAD.IsYPressed()) {
		command |= PAD_TRG4;
	}

	return command;
}

void DrawString(int x, int y, char *buf, UINTCOLOR color, int format, int sizeX, int sizeY, float angle)
{
	SpriteString::DrawString(framework::s_pDeviceContext, x, y, buf, XMConvertUIntToColor(color), format, sizeX, sizeY, angle);
}

void DrawRectangle(int x, int y, int w, int h, float angle, UINTCOLOR color)
{
	static Sprite rect(framework::s_pDevice);
	//MyBlending::setMode(framework::pDeviceContext, BLEND_REPLACE);
	rect.Draw(framework::s_pDeviceContext, x, y, w, h, angle, XMConvertUIntToColor(color));
	//MyBlending::setMode(framework::pDeviceContext, BLEND_ALPHA);
}


///////////////////////////////////////////////////////////////
// Class View
View::View(int viewWidth, int viewHeight) :doReflection(false)
{
	pRenderTarget = new RenderTarget(framework::s_pDevice, viewWidth, viewHeight);
	transform.Clear();
}

View::View(float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, bool doReflection):
drawX(drawX), drawY(drawY), drawWidth(drawWidth), drawHeight(drawHeight), 
srcX(srcX), srcY(srcY), srcWidth(srcWidth), srcHeight(srcHeight),
rotateAngle(rotateAngle),
blendColor(blendColor),
doReflection(doReflection)
{
	pRenderTarget = new RenderTarget(framework::s_pDevice, drawWidth, drawHeight);
	transform.Clear();
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
	static XMMATRIX world, view, projection;
	//world = view = projection = DirectX::XMMatrixIdentity();
	const XMVECTORF32 translate = { transform.position.x, transform.position.y, transform.position.z };
	const XMVECTORF32 scale = { transform.scaling.x, transform.scaling.y, transform.scaling.z };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(transform.eulerDegreeAngle.x, transform.eulerDegreeAngle.y, transform.eulerDegreeAngle.z);

	world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
	view = DirectX::XMMatrixLookAtLH(Camera::mainCamera.eyePosition, Camera::mainCamera.focusPosition, Camera::mainCamera.upDirection);
	projection = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);

	pRenderTarget->Draw(framework::s_pDeviceContext, world, view, projection, drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, XMConvertUIntToColor(blendColor), doReflection);
}

void View::Set()
{
	static XMMATRIX world, view, projection;
	//world = view = projection = DirectX::XMMatrixIdentity();
	const XMVECTORF32 translate = { transform.position.x, transform.position.y, transform.position.z };
	const XMVECTORF32 scale = { transform.scaling.x, transform.scaling.y, transform.scaling.z };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(transform.eulerDegreeAngle.x, transform.eulerDegreeAngle.y, transform.eulerDegreeAngle.z);

	world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
	view = DirectX::XMMatrixLookAtLH(Camera::mainCamera.eyePosition, Camera::mainCamera.focusPosition, Camera::mainCamera.upDirection);
	projection = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);

	pRenderTarget->Draw(framework::s_pDeviceContext, world, view, projection, drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, XMConvertUIntToColor(blendColor), doReflection);
}

void View::Clear()
{
	framework::s_pDeviceContext->OMSetRenderTargets(1, &framework::s_pRenderTargetView, framework::s_pDepthStencilView);

	Camera::mainCamera.viewPort.Width = SCREEN_WIDTH;
	Camera::mainCamera.viewPort.Height = SCREEN_HEIGHT;
	framework::s_pDeviceContext->RSSetViewports(1, &Camera::mainCamera.viewPort);
}

///////////////////////////////////////////////////////////

// Class Cube
Cube::Cube(const XMFLOAT3 &position, const XMFLOAT3 &size, const UINTCOLOR &blendColor) :position(position), size(size), blendColor(blendColor)
{
	pPrimitive = new Primitive3D(framework::s_pDevice);
	pPrimitive->Initialize(framework::s_pDevice, GEOMETRY_CUBE);
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

void MeshData::Draw(const Transform &transform, const int& frame)
{
	if (fileNO >= 0 && fileNO < MAX_MESH_FILE_NUM && pMeshManager->meshAt(fileNO) && pMeshManager->meshAt(fileNO)->data) {
		// TODO: Use Matrix in Scene

		Vector3 positionAdd = preSetTransform.position + transform.position;
		Vector3 scaleMul = preSetTransform.scaling * transform.scaling;
		Vector3 rotateAdd = (preSetTransform.eulerDegreeAngle + transform.eulerDegreeAngle)*0.01745f;

		static XMMATRIX world, view, projection;
		//world = view = projection = DirectX::XMMatrixIdentity();
		const XMVECTORF32 translate = { positionAdd.x, positionAdd.y, positionAdd.z };
		const XMVECTORF32 scale = { scaleMul.x, scaleMul.y, scaleMul.z };
		XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(rotateAdd.x, rotateAdd.y, rotateAdd.z);

		world = XMMatrixTransformation(g_XMZero, XMQuaternionIdentity(), scale, g_XMZero, rotate, translate);
		view = DirectX::XMMatrixLookAtLH(Camera::mainCamera.eyePosition, Camera::mainCamera.focusPosition, Camera::mainCamera.upDirection);
		projection = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);

		pMeshManager->meshAt(fileNO)->data->Draw(framework::s_pDeviceContext, world, view, projection, false, frame);
	}
}

void MeshManager::LoadMesh(MeshFile data[], int fileNO)
{
	if (data[fileNO].fileNO != -1 || data[fileNO].path != NULL)
	{
		meshes[fileNO] = new MeshFile(data[fileNO].fileNO, data[fileNO].path);
		meshes[fileNO]->data = new SkinnedMesh(framework::s_pDevice, data[fileNO].path);
	}
}

void MeshManager::LoadMeshes(MeshFile data[])
{
	for (int i = data[0].fileNO; data[i].fileNO != -1 || data[i].path != NULL; i++)
	{
		meshes[i] = new MeshFile(data[i].fileNO, data[i].path);
		meshes[i]->data = new SkinnedMesh(framework::s_pDevice, data[i].path);
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
		meshes[i]->data = new SkinnedMesh(framework::s_pDevice, data[i].path);
		std::fstream file(data[i].path, std::ios::in);
		file.seekg(0, std::ios::end);
		loadedMeshSize+= file.tellg();
		*progress = loadedMeshSize / (float)meshesSize * 100;
		/*GetFileSizeEx(data[i].path, &fileSize);
		loadedMeshSize.QuadPart += fileSize.QuadPart;
		*progress = loadedMeshSize.QuadPart / meshesSize.QuadPart * 100;*/
	}
}

const MeshFile* MeshManager::meshAt(int fileNO)
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
			ZeroMemory(meshes[i], sizeof(*meshes[i]));
		}
	}
	ZeroMemory(meshes, sizeof(MeshFile)*MAX_MESH_FILE_NUM);
}

////////////////////////////////////////////////////////////