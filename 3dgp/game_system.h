#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include <cmath>
#include "resources_manager.h"
#include "sprite_string.h"
#include "framework.h"
#include "camera.h"
#include "input.h"

#include "3dgp_math.h"


//#define _XM_NO_INTRINSICS_ 
#include <DirectXMath.h>

//*****************************************************************************************************************************
//
//	define定義
//
//*****************************************************************************************************************************
// キーラベル
const int	PAD_UP		= (1 << 0);
const int	PAD_DOWN	= (1 << 1);
const int	PAD_LEFT	= (1 << 2);
const int	PAD_RIGHT	= (1 << 3);
const int	PAD_TRG1	= (1 << 4);
const int	PAD_TRG2	= (1 << 5);
const int	PAD_TRG3	= (1 << 6);
const int	PAD_TRG4	= (1 << 7);
const int	PAD_START	= (1 << 8);
const int	PAD_BACK	= (1 << 9);
const int	PAD_L1		= (1 << 10);
const int	PAD_R1		= (1 << 11);
const int	PAD_L2		= (1 << 12);
const int	PAD_R2		= (1 << 13);
const int	PAD_L3		= (1 << 14);
const int	PAD_R3		= (1 << 15);

const int	COLOR_WHITE		= 0xFFFFFFFF;
const int	COLOR_BLACK		= 0x000000FF;
const int	COLOR_GRAY		= 0x808080FF;
const int	COLOR_RED		= 0xFF0000FF;
const int	COLOR_ORANGE	= 0xFFA500FF;
const int	COLOR_YELLOW	= 0xFFFF00FF;
const int	COLOR_GREEN		= 0x008000FF;
const int	COLOR_BLUE		= 0x0000FFFF;
const int	COLOR_CYAN		= 0x00FFFFFF;
const int	COLOR_VIOLET	= 0x800080FF;

#define TEX_MAX (RM::FILE_NUM_MAX)

enum STEP
{
	INIT = 0,
	BEGIN = 5,

	END = 20,

	FINISH = 40,
};


template<typename Resource, size_t AMOUNT>
class ResourceManager
{
private:
	Resource *pResource[AMOUNT];

	ResourceManager() {};
	~ResourceManager() {
		Release();
	};


public:
	void Load(Resource resources[]);
	const Resource* At(int resourceNO);


	void Release();

	static ResourceManager* GetInstance() {
		static ResourceManager instance;
		return &instance;
	};

};


#define pTextureManager (ResourceManager<GLC::Texture, TEX_MAX>::GetInstance())

// Keyboard: Up, Down, Left, Right, Enter, Back, Z, X, C, V
// Controller: Up, Down, Left, Right, Start, Back, A, B, X, Y
// All of the buttons are Triggers except Up, Down, Left, Right.
int BasicInput(int controllerNO = 0);

// Draw a simple text use default ascii font texture.
// @format(STR_LEFT or STR_CENTER), @textColor(RGBA)
void DrawString(int posX = 0, int posY = 0, char *pTextBuf = nullptr, GLC::UINTCOLOR textColor = 0xFFFFFFFF, int format = STR_LEFT, int characterSizeX = 32, int characterSizeY = 32, float characterRotateAngle = .0f);

//void DrawRectangle(int leftTopX, int leftTopY, int width, int height, float rotateAngle = 0.0, GLC::UINTCOLOR fillColor = 0xFFFFFFFF);


//class View: extend from class RenderTarget
class RenderTarget;
class View
{
private:
	RenderTarget *pRenderTarget;
	int drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight;
	float rotateAngle;
	GLC::UINTCOLOR blendColor;


public:

	View(int viewWidth, int viewHeight);
	View(float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, GLC::UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);
	~View();

	bool doReflection;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;

	void Set();
	// View, looks like a 3D textured sprite
	void Set(float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, GLC::UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);

	// Reset ViewPort to real screen
	static void Clear();
};


// Class Cube: extend from class Primitive3d
class Primitive3D;
class Cube
{
private:
	Primitive3D *pPrimitive;
public:
	Cube(DirectX::FXMVECTOR position, DirectX::FXMVECTOR size, const GLC::UINTCOLOR &blendColor);
	~Cube();
	DirectX::XMVECTOR	position;
	DirectX::XMVECTOR	size;
	GLC::UINTCOLOR	blendColor;

	void Draw();

};

// TODO: Review the Naming consistency 

// Skinned Mesh Data Management

class SkinnedMesh;

struct MeshFile
{
	int fileNO = -1;
	char* path = nullptr;
	SkinnedMesh* data = nullptr;
	MeshFile() {};
	MeshFile(int fileNO, char* filePath) :fileNO(fileNO), path(filePath) {};
	/*const MeshFile& operator=(const MeshFile& rhv) {
		path = rhv.path;
		return *this;
	};*/
	
};


__declspec(align(16)) struct MeshData
{
	int fileNO;

	DirectX::XMVECTOR scaleAdjustion;
	DirectX::XMVECTOR positionAdjustion;
	DirectX::XMVECTOR rotationAdjustion;
	DirectX::XMMATRIX world;

	MeshData(int fileNO, 
		DirectX::XMFLOAT3 preSetScale = DirectX::XMFLOAT3(1, 1, 1), 
		DirectX::XMFLOAT3 preSetPosition = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 preSetRotationDegree = DirectX::XMFLOAT3(0, 0, 0)) 
		:fileNO(fileNO), 
		world(DirectX::XMMatrixIdentity())
	{
		scaleAdjustion = DirectX::XMLoadFloat3(&preSetScale);
		positionAdjustion = DirectX::XMLoadFloat3(&preSetPosition);
		rotationAdjustion = DirectX::XMLoadFloat3(&preSetRotationDegree);
	};
	void SetMotion(int fbxFileNO);
	void XM_CALLCONV Draw(DirectX::FXMVECTOR position, DirectX::FXMVECTOR scale, DirectX::FXMVECTOR rotation, int* frame = nullptr);

};

#define MAX_MESH_FILE_NUM (64)

class MeshManager
{
private:
	MeshFile* meshes[MAX_MESH_FILE_NUM];

	MeshManager() {};
	~MeshManager() {
		ReleaseMeshes();
	};

public:
	void LoadMeshes(MeshFile sequencedData[]);
	void LoadMeshes(MeshFile sequencedData[],int *progress);
	void LoadMesh(MeshFile sequencedData[], int fileNO);

	const MeshFile* MeshAt(int fileNO);

	void ReleaseMeshes();

	static MeshManager* GetInstance() {
		static MeshManager instance;
		return &instance;
	};

};

#define pMeshManager (MeshManager::GetInstance())


#endif // !_GAME_SYSTEM_H_