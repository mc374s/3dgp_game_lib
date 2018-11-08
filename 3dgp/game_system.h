#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_


#include <cmath>
#include "sprite_string.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

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

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x0001 ? 1 : 0))
#define KEY_PRESS(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ?1 : 0))
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1))
//#define KEY_CLICK(vk_code) ((GetAsyncKeyState(vk_code) & 0x0001 ? 1 : 0))
//#define KEY_PRESS(vk_code) ((GetAsyncKeyState(vk_code) & 0x8001 ))
//#define KEY_RELEASE(vk_code) ((GetAsyncKeyState(vk_code) == 0))


#define TEX_MAX (FILE_NUM_MAX)

enum STEP
{
	INIT = 0,
	BEGIN = 5,

	END = 20,

	FINISH = 40,
};

class Sprite;

struct LOAD_TEXTURE
{
	int		texNO;
	char	*fileName;
	bool	doProjection;	// 3d空間で描画するかどうか
	Sprite* img;
	LOAD_TEXTURE(int texNum, char *pFileName, bool doProjection = false) :texNO(texNum), fileName(pFileName), doProjection(doProjection) {};
};


struct Transform2D
{
	float	scaleX, scaleY;
	float	angle;
	bool	reflectX;
	bool	centRotate;
	float	centX, centY;
	int		scaleMode;

	UINTCOLOR rgba;

	Transform2D(float scaleX = 1.0f, float scaleY = 1.0f, float angle = .0f, bool reflectX = false, bool centRotate = true, float centX = .0f, float centY = .0f, int scaleMode = 0, UINTCOLOR rgba = 0xFFFFFFFF) :
		scaleX(scaleX),
		scaleY(scaleY),
		angle(angle),
		reflectX(reflectX),
		centRotate(centRotate),
		centX(centX),
		centY(centY),
		scaleMode(scaleMode),
		rgba(rgba)
	{};

	void clear()
	{
		scaleX = 1.0f;
		scaleY = 1.0f;
		angle = .0f;
		reflectX = false;
		centRotate = true;
		centX = .0f;
		centY = .0f;
		scaleMode = 0;
		rgba = 0xFFFFFFFF;
	};

	static Transform2D initialValue() 
	{
		Transform2D clearedValue;
		clearedValue.clear();
		return clearedValue;
	}
};

struct SPRITE_DATA
{
	int		texNO = 0;
	float	left, top;
	float	width, height;
	float	ofsX, ofsY;
	int	frameNum;
	SPRITE_DATA(int texNum = -1, float left = 0, float top = 0, float width = 0, float height = 0, float ofsX = 0, float ofsY = 0, int frameNum = 1) :
		texNO(texNum),
		left(left),
		top(top),
		width(width),
		height(height),
		ofsX(ofsX),
		ofsY(ofsY),
		frameNum(frameNum)
	{};
	void draw(Vector3 &pos, const Transform2D& transform2d = Transform2D::initialValue(), const Transform& transform=Transform::initialValue());
	void draw(float x, float y, const Transform2D& transform2d = Transform2D::initialValue());
	void copy(const SPRITE_DATA* rhv) {
		left = rhv->left; top = rhv->top; width = rhv->width; height = rhv->height; ofsX = rhv->ofsX; ofsY = rhv->ofsY;
		//return *this;
	};

};

typedef SPRITE_DATA SPRITE_LEFTTOP;

struct SPRITE_CENTER : public SPRITE_DATA
{
	SPRITE_CENTER(int texNO, float left, float top, float width, float height, int frameNum = 1) : SPRITE_DATA(texNO, left, top, width, height, -width / 2, -height / 2, frameNum) {};
};

struct SPRITE_BOTTOM : public SPRITE_DATA
{
	SPRITE_BOTTOM(int texNO, float left, float top, float width, float height, int frameNum = 1) : SPRITE_DATA(texNO, left, top, width, height, -width / 2, -height, frameNum) {};
};


class TextureManager
{
private:
	LOAD_TEXTURE *pTextures[TEX_MAX];

	TextureManager() {};
	~TextureManager() {
		releaseTexture();
	};


public:
	void loadTextures(LOAD_TEXTURE data[]);
	void loadTexture(LOAD_TEXTURE data[], int textureNO);
	const LOAD_TEXTURE* textureAt(int fileNO);


	void releaseTexture();

	static TextureManager* getInstance() {
		static TextureManager instance;
		return &instance;
	};

};

#define pTextureManager (TextureManager::getInstance())

int basicInput();

void drawString(int posX = 0, int posY = 0, char *pTextBuf = nullptr, UINTCOLOR textColor = 0xFFFFFFFF, int format = STR_LEFT, int characterSizeX = 32, int characterSizeY = 32, float characterRotateAngle = .0f);

void drawRectangle(int leftTopX, int leftTopY, int width, int height, float rotateAngle = 0.0, UINTCOLOR fillColor = 0xFFFFFFFF);



//class View: extend from class RenderTarget
class RenderTarget;
class View
{
private:
	RenderTarget *pRenderTarget;
	int drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight;
	float rotateAngle;
	UINTCOLOR blendColor;


public:

	View(int viewWidth, int viewHeight);
	View(float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);
	~View();

	bool doReflection;
	Transform transform;

	void set();
	// View, looks like a 3D textured sprite
	void set(float drawX, float drawY, float drawWidth, float drawHeight, float srcX = .0f, float srcY = .0f, float srcWidth = .0f, float srcHeight = .0f, float rotateAngle = .0f, UINTCOLOR blendColor = 0xFFFFFFFF, bool doReflection = false);

	// Reset ViewPort to real screen
	static void clear();
};


// Class Cube: extend from class Primitive3d
class Primitive3D;
class Cube
{
private:
	Primitive3D *pPrimitive;
public:
	Cube(const XMFLOAT3 &position, const XMFLOAT3 &size, const UINTCOLOR &blendColor);
	~Cube();
	XMFLOAT3	position;
	XMFLOAT3	size;
	UINTCOLOR	blendColor;
	Transform	transform;

	void draw();

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


struct MeshData
{
	int fileNO;
	Transform preSetTransform;
	MeshData(int fileNO, XMFLOAT3 preSetScale = XMFLOAT3(1, 1, 1), XMFLOAT3 preSetPosition = XMFLOAT3(0, 0, 0), XMFLOAT3 preSetEulerAngle = XMFLOAT3(0, 0, 0))
		:fileNO(fileNO) {
		preSetTransform.scaling = preSetScale;
		preSetTransform.position = preSetPosition;
		preSetTransform.eulerAngle = preSetEulerAngle;
	};
	void draw(const Transform &transform = Transform::initialValue(), const int& frame = 0);

};

#define MAX_MESH_FILE_NUM (64)

class MeshManager
{
private:
	MeshFile* meshes[MAX_MESH_FILE_NUM];

	MeshManager() {};
	~MeshManager() {
		releaseMeshes();
	};

public:
	void loadMeshes(MeshFile sequencedData[]);
	void loadMeshes(MeshFile sequencedData[],int *progress);
	void loadMesh(MeshFile sequencedData[], int fileNO);

	const MeshFile* meshAt(int fileNO);

	void releaseMeshes();

	static MeshManager* getInstance() {
		static MeshManager instance;
		return &instance;
	};

};

#define pMeshManager (MeshManager::getInstance())



#endif // !_GAME_SYSTEM_H_