#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_


#include <cmath>
#include "sprite_string.h"

typedef unsigned int D3DCOLOR;
//*****************************************************************************
//		３Ｄベクトル
//*****************************************************************************
//------------------------------------------------------
//	３Ｄベクトル基本構造体
//------------------------------------------------------
typedef struct Vector {
	float	x, y, z;
} Vector;

//------------------------------------------------------
//	３Ｄベクトル構造体
//------------------------------------------------------
typedef struct Vector3 : public Vector
{
public:
	//	コンストラクタ
	Vector3() {};
	inline Vector3(float x, float y, float z) { this->x = x, this->y = y, this->z = z; }
	inline Vector3(const Vector& v) { this->x = v.x, this->y = v.y, this->z = v.z; }

	//	距離計算
	inline float Length() { return sqrtf(x*x + y*y + z*z); }
	inline float LengthSq() { return x*x + y*y + z*z; }

	//	正規化
	void Normalize()
	{
		float l = Length();
		if (fabsf(l - 0.0f) > FLT_EPSILON) { x /= l; y /= l; z /= l; }
	}

	//	オペレーター
	inline Vector3& operator = (const Vector& v) { x = v.x; y = v.y; z = v.z; return *this; }
	inline Vector3& operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	inline Vector3& operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline Vector3& operator *= (float v) { x *= v; y *= v; z *= v; return *this; }
	inline Vector3& operator /= (float v) { x /= v; y /= v; z /= v; return *this; }

	inline Vector3 operator + () const { Vector3 ret(x, y, z); return ret; }
	inline Vector3 operator - () const { Vector3 ret(-x, -y, -z); return ret; }

	inline Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator * (float v) const { Vector3 ret(x*v, y*v, z*v); return ret; }
	inline Vector3 operator / (float v) const { Vector3 ret(x / v, y / v, z / v); return ret; }

	bool operator == (const Vector3& v) const { return (fabsf(x - v.x) < FLT_EPSILON) && (fabsf(y - v.y) < FLT_EPSILON) && (fabsf(z - v.z) < FLT_EPSILON); }
	bool operator != (const Vector3& v) const { return (fabsf(x - v.x) > FLT_EPSILON) || (fabsf(y - v.y) > FLT_EPSILON) || (fabsf(z - v.z) > FLT_EPSILON); }

} Vector3;

//------------------------------------------------------
//	外積
//------------------------------------------------------
inline void Vector3Cross(Vector& out, Vector& v1, Vector& v2)
{
	out.x = v1.y*v2.z - v1.z*v2.y;
	out.y = v1.z*v2.x - v1.x*v2.z;
	out.z = v1.x*v2.y - v1.y*v2.x;
}

//------------------------------------------------------
//	内積
//------------------------------------------------------
inline float Vector3Dot(Vector& v1, Vector& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// edit by ChenYuezong 2018/05/25
// operator " = [] () -> " must be defined as memeber operator

XMFLOAT3& operator += (XMFLOAT3& lhv, const XMFLOAT3& rhv);
XMFLOAT3& operator -= (XMFLOAT3& lhv, const XMFLOAT3& rhv);
XMFLOAT3& operator *= (XMFLOAT3& lhv, float rhv);
XMFLOAT3& operator /= (XMFLOAT3& lhv, float rhv);

//inline XMFLOAT3 operator + () const { XMFLOAT3 ret(x, y, z); return ret; }
//inline XMFLOAT3 operator - () const { XMFLOAT3 ret(-x, -y, -z); return ret; }

const XMFLOAT3 operator + (const XMFLOAT3& lhv, const XMFLOAT3& rhv);
const XMFLOAT3 operator - (const XMFLOAT3& lhv, const XMFLOAT3& rhv);
const XMFLOAT3 operator * (const XMFLOAT3& lhv, float rhv);
const XMFLOAT3 operator / (const XMFLOAT3& lhv, float rhv);

bool const operator == (const XMFLOAT3& lhv, const XMFLOAT3& rhv);
bool const operator != (const XMFLOAT3& lhv, const XMFLOAT3& rhv);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
const int	PAD_START	= (1 << 4);
const int	PAD_TRG1	= (1 << 5);
const int	PAD_TRG2	= (1 << 6);
const int	PAD_TRG3	= (1 << 7);
const int	PAD_ENTER	= (1 << 8);
const int	PAD_SELECT	= (1 << 8);
const int	PAD_TRG4	= (1 << 9);
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
	int		texNO;			// テクスチャ番号
	char	*fileName;		// ファイル名
	bool	doProjection;	// 3dで描画するか
	Sprite* img;
	LOAD_TEXTURE(int a_texNum, char *a_pFileName, bool a_doProjection = false) :texNO(a_texNum), fileName(a_pFileName), doProjection(a_doProjection) {};
};


struct CUSTOM
{
	float	scaleX, scaleY;
	float	angle;
	bool	reflectX;
	bool	centRotate;
	float	centX, centY;
	int		scaleMode;

	UINTCOLOR rgba;

	CUSTOM(float _scaleX = 1.0f, float _scaleY = 1.0f, float _angle = .0f, bool _reflectX = false, bool _centRotate = true, float _centX = .0f, float _centY = .0f, int _scaleMode = 0, UINTCOLOR _rgba = 0xFFFFFFFF)
	{
		scaleX = _scaleX;
		scaleY = _scaleY;
		angle = _angle;
		reflectX = _reflectX;
		centRotate = _centRotate;
		centX = _centX;
		centY = _centY;
		scaleMode = _scaleMode;
		rgba = _rgba;
	};

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

};

struct SPRITE_DATA
{
	int		texNO = 0;
	float	left, top;
	float	width, height;
	float	ofsX, ofsY;
	int	frameNum;
	SPRITE_DATA(int _texNum = -1, float _left = 0, float _top = 0, float _width = 0, float _height = 0, float _ofsX = 0, float _ofsY = 0, int _frameNum = 1) {
		texNO = _texNum;
		left = _left;
		top = _top;
		width = _width;
		height = _height;
		ofsX = _ofsX;
		ofsY = _ofsY;
		frameNum = _frameNum;
	};
	void draw(Vector3 &a_pos, CUSTOM *a_pCustom = nullptr, CUSTOM3D *a_pCustom3d = nullptr);
	void draw(float _x, float _y, CUSTOM *_custom = nullptr);
	void copy(const SPRITE_DATA* a_rhv) {
		left = a_rhv->left; top = a_rhv->top; width = a_rhv->width; height = a_rhv->height; ofsX = a_rhv->ofsX; ofsY = a_rhv->ofsY;
		//return *this;
	};

};

typedef SPRITE_DATA SPRITE_LEFTTOP;

struct SPRITE_CENTER : public SPRITE_DATA
{
	SPRITE_CENTER(int texNO, float left, float top, float width, float height, int _frameNum = 1) : SPRITE_DATA(texNO, left, top, width, height, -width / 2, -height / 2, _frameNum) {};
};

struct SPRITE_BOTTOM : public SPRITE_DATA
{
	SPRITE_BOTTOM(int texNO, float left, float top, float width, float height, int _frameNum = 1) : SPRITE_DATA(texNO, left, top, width, height, -width / 2, -height, _frameNum) {};
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
	void loadTextures(LOAD_TEXTURE _data[]);
	void loadTexture(LOAD_TEXTURE _data[], int a_textureNO);
	const LOAD_TEXTURE* textureAt(int fileNO);


	void releaseTexture();

	static TextureManager* getInstance() {
		static TextureManager instance;
		return &instance;
	};

};

#define pTextureManager (TextureManager::getInstance())

int getInputKey();

void drawString(int a_posX = 0, int a_posY = 0, char *a_pTextBuf = nullptr, UINTCOLOR a_textColor = 0xFFFFFFFF, int _format = STR_LEFT, int a_characterSizeX = 32, int a_characterSizeY = 32, float a_characterRotateAngle = .0f);

void drawRectangle(int a_leftTopX, int a_leftTopY, int a_width, int a_height, float a_rotateAngle = 0.0, UINTCOLOR a_fillColor = 0xFFFFFFFF);



//class View: extend from class RenderTarget
class RenderTarget;
class View
{
private:
	RenderTarget *m_pRenderTarget;
	int m_drawX, m_drawY, m_drawWidth, m_drawHeight, m_srcX, m_srcY, m_srcWidth, m_srcHeight;
	float m_rotateAngle;
	UINTCOLOR m_blendColor;


public:

	View(int a_viewWidth, int a_viewHeight);
	View(float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX = .0f, float a_srcY = .0f, float a_srcWidth = .0f, float a_srcHeight = .0f, float a_rotateAngle = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, bool a_doReflection = false);
	~View();

	bool m_doReflection;
	CUSTOM3D m_custom3d;

	void set();
	// View, looks like a 3D textured sprite
	void set(float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX = .0f, float a_srcY = .0f, float a_srcWidth = .0f, float a_srcHeight = .0f, float a_rotateAngle = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, bool a_doReflection = false);

	// Reset ViewPort to real screen
	static void clear();
};


// Class Cube: extend from class Primitive3d
class Primitive3D;
class Cube
{
private:
	Primitive3D *m_pPrimitive;
public:
	Cube(const XMFLOAT3 &a_position, const XMFLOAT3 &a_size, const UINTCOLOR &a_blendColor);
	~Cube();
	XMFLOAT3	m_position;
	XMFLOAT3	m_size;
	UINTCOLOR	m_blendColor;
	CUSTOM3D	m_custom3d;

	void draw();

};


// Skinned Mesh Data Managment

class SkinnedMesh;

struct LOAD_MESH
{
	int meshNO;
	char* pFileName;
	SkinnedMesh* pData;
	LOAD_MESH(int meshNO, char* pFileName) :meshNO(meshNO), pFileName(pFileName) {};
};


struct MESH_DATA
{
	int meshNO;
	XMFLOAT3 eulerAngle;
	XMFLOAT3 offSet;
	MESH_DATA(int meshNO, XMFLOAT3 eulerAngle, XMFLOAT3 offSet);
	void draw(XMFLOAT3 position, const CUSTOM3D &custom = CUSTOM3D::initialValue());

};

class MeshManager
{
private:


	MeshManager() {};
	~MeshManager() {
		releaseMeshes();
	};

public:

	void loadMeshes(MESH_DATA data[]);
	void loadMesh(MESH_DATA data[], int meshNO);

	void releaseMeshes();

	static MeshManager* getInstance() {
		static MeshManager instance;
		return &instance;
	};

};

#define pMeshManager (MeshManager::getInstance())

#endif // !_GAME_SYSTEM_H_