#include "game_system.h"

#include "framework.h"
#include "blend.h"
#include "sprite.h"

#include "skinned_mesh.h"

#include <fstream>


void SPRITE_DATA::draw(float x, float y,const Transform2D& transform2D) {
	if (texNO >= 0 && texNO < TEX_MAX && pTextureManager->textureAt(texNO) && pTextureManager->textureAt(texNO)->img)
	{
		pTextureManager->textureAt(texNO)->img->render(framework::s_pDeviceContext, x + ofsX, y + ofsY, transform2D.scaleX*width, transform2D.scaleY*height, left, top, width, height, transform2D.rgba, transform2D.angle, transform2D.centRotate, transform2D.centX, transform2D.centY, transform2D.reflectX, transform2D.scaleMode);
	}
}

void SPRITE_DATA::draw(Vector3 &pos, const Transform2D& transform2D, const Transform& transfrom) {
	if (texNO >= 0 && texNO < TEX_MAX && pTextureManager->textureAt(texNO) && pTextureManager->textureAt(texNO)->img) {
		if (pTextureManager->textureAt(texNO)->doProjection)
		{
			pTextureManager->textureAt(texNO)->img->render3D(framework::s_pDeviceContext, pos.x + ofsX, pos.y + ofsY, transform2D.scaleX*width, transform2D.scaleY*height, left, top, width, height, transform2D.rgba, transform2D.angle, transform2D.centRotate, transform2D.centX, transform2D.centY, transform2D.reflectX, transform2D.scaleMode, transfrom);
		}
		else
		{
			draw(pos.x, pos.y, transform2D);
		}
	}
}

void TextureManager::loadTextures(LOAD_TEXTURE data[])
{
	for (int i = data[0].texNO; data[i].texNO != -1 || data[i].fileName != NULL; i++)
	{
		data[i].img = new Sprite(framework::s_pDevice, data[i].fileName, data[i].doProjection);
		pTextures[i] = &data[i];
	}
}

void TextureManager::loadTexture(LOAD_TEXTURE data[], int textureNO)
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

void TextureManager::releaseTexture()
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


int  basicInput()
{
	int command = 0x0;

	if (KEY_BOARD.Up || GAME_PAD.IsLeftThumbStickUp()) {
		command |= PAD_UP;
	}
	if (KEY_BOARD.Down || GAME_PAD.IsLeftThumbStickDown()) {
		command |= PAD_DOWN;
	}
	if (KEY_BOARD.Left || GAME_PAD.IsLeftThumbStickLeft()){
		command |= PAD_LEFT;
	}
	if (KEY_BOARD.Right || GAME_PAD.IsLeftThumbStickRight()){
		command |= PAD_RIGHT;
	}
	if (KEY_TRACKER.pressed.Enter || GAME_PAD.IsStartPressed()) {
		command |= PAD_START;
	}
	if (KEY_TRACKER.pressed.Escape || GAME_PAD.IsBackPressed()) {
		command |= PAD_BACK;
	}
	if (KEY_TRACKER.pressed.Z || GAME_PAD.IsAPressed()) {
		command |= PAD_TRG1;
	}
	if (KEY_TRACKER.pressed.X || GAME_PAD.IsBPressed()) {
		command |= PAD_TRG2;
	}
	if (KEY_TRACKER.pressed.C || GAME_PAD.IsXPressed()) {
		command |= PAD_TRG3;
	}
	if (KEY_TRACKER.pressed.V || GAME_PAD.IsYPressed()) {
		command |= PAD_TRG4;
	}

	return command;
}

void drawString(int x, int y, char *buf, UINTCOLOR color, int format, int sizeX, int sizeY, float angle)
{
	drawSprString(framework::s_pDeviceContext, x, y, buf, color, format, sizeX, sizeY, angle);
}

void drawRectangle(int x, int y, int w, int h, float angle, UINTCOLOR color)
{
	static Sprite rect(framework::s_pDevice);
	//MyBlending::setMode(framework::pDeviceContext, BLEND_REPLACE);
	rect.render(framework::s_pDeviceContext, x, y, w, h, angle, color);
	//MyBlending::setMode(framework::pDeviceContext, BLEND_ALPHA);
}


///////////////////////////////////////////////////////////////
// Class View
View::View(int viewWidth, int viewHeight) :doReflection(false)
{
	pRenderTarget = new RenderTarget(framework::s_pDevice, viewWidth, viewHeight);
	transform.clear();
}

View::View(float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, bool doReflection):
drawX(drawX), drawY(drawY), drawWidth(drawWidth), drawHeight(drawHeight), 
srcX(srcX), srcY(srcY), srcWidth(srcWidth), srcHeight(srcHeight),
rotateAngle(rotateAngle),
blendColor(blendColor),
doReflection(doReflection)
{
	pRenderTarget = new RenderTarget(framework::s_pDevice, drawWidth, drawHeight);
	transform.clear();
}

View::~View()
{
	if (pRenderTarget)
	{
		delete pRenderTarget;
		pRenderTarget = NULL;
	}
}

void View::set(float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, bool doReflection)
{
	pRenderTarget->render3D(framework::s_pDeviceContext, drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, blendColor, transform, doReflection);
}

void View::set()
{
	pRenderTarget->render3D(framework::s_pDeviceContext, drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, blendColor, transform, doReflection);
}

void View::clear()
{
	framework::s_pDeviceContext->OMSetRenderTargets(1, &framework::s_pRenderTargetView, framework::s_pDepthStencilView);

	e_mainCamera.viewPort.Width = SCREEN_WIDTH;
	e_mainCamera.viewPort.Height = SCREEN_HEIGHT;
	framework::s_pDeviceContext->RSSetViewports(1, &e_mainCamera.viewPort);
	setRenderTargetWH(SCREEN_WIDTH, SCREEN_HEIGHT);
}

///////////////////////////////////////////////////////////

// Class Cube
Cube::Cube(const XMFLOAT3 &position, const XMFLOAT3 &size, const UINTCOLOR &blendColor) :position(position), size(size), blendColor(blendColor)
{
	pPrimitive = new Primitive3D(framework::s_pDevice);
	pPrimitive->initialize(framework::s_pDevice, GEOMETRY_CUBE);
}

Cube::~Cube()
{
	if (pPrimitive)
	{
		delete pPrimitive;
		pPrimitive = NULL;
	}
}

void Cube::draw()
{
	pPrimitive->drawCube(framework::s_pDeviceContext, position, size, blendColor, transform);
}

////////////////////////////////////////////////////////////

// Skinned Mesh Data Management

void MeshData::draw(const Transform &transform, const int& frame)
{
	if (fileNO >= 0 && fileNO < MAX_MESH_FILE_NUM && pMeshManager->meshAt(fileNO) && pMeshManager->meshAt(fileNO)->data) {
		pMeshManager->meshAt(fileNO)->data->drawMesh(framework::s_pDeviceContext, preSetTransform, transform, frame);
	}
}

void MeshManager::loadMesh(MeshFile data[], int fileNO)
{
	if (data[fileNO].fileNO != -1 || data[fileNO].path != NULL)
	{
		meshes[fileNO] = new MeshFile(data[fileNO].fileNO, data[fileNO].path);
		meshes[fileNO]->data = new SkinnedMesh(framework::s_pDevice, data[fileNO].path);
	}
}

void MeshManager::loadMeshes(MeshFile data[])
{
	for (int i = data[0].fileNO; data[i].fileNO != -1 || data[i].path != NULL; i++)
	{
		meshes[i] = new MeshFile(data[i].fileNO, data[i].path);
		meshes[i]->data = new SkinnedMesh(framework::s_pDevice, data[i].path);
	}
}

void MeshManager::loadMeshes(MeshFile data[], int *progress)
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

void MeshManager::releaseMeshes()
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