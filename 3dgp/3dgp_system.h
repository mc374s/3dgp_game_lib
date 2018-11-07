#ifndef  _3DGPSYSTEM_H_
#define  _3DGPSYSTEM_H_

#pragma comment(lib,"d3d11")
#pragma comment(lib,"dxgi")
#pragma comment(lib,"DirectXTK")
#pragma comment(lib,"libfbxsdk-md")

#define _XM_NO_INTRINSICS_

#include <windows.h>
#include <memory>
#include <d3d11.h>
#include <tchar.h>
#include <sstream>
#include <assert.h>
#include <tchar.h>
#include <ctime>
#include <cstdlib>

#include <mfplay.h>

#include "pch.h"

#include "misc.h"
#include "high_resolution_timer.h"

#include "resources_manager.h"

#define _TEST (0)

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

using namespace DirectX;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// edit by ChenYuezong 2018/05/25
// operator " = [] () -> " must be defined as memeber operator

//XMFLOAT3& operator += (XMFLOAT3& lhv, const XMFLOAT3& rhv);
//XMFLOAT3& operator -= (XMFLOAT3& lhv, const XMFLOAT3& rhv);
//XMFLOAT3& operator *= (XMFLOAT3& lhv, float rhv);
//XMFLOAT3& operator /= (XMFLOAT3& lhv, float rhv);
//
////inline XMFLOAT3 operator + () const { XMFLOAT3 ret(x, y, z); return ret; }
////inline XMFLOAT3 operator - () const { XMFLOAT3 ret(-x, -y, -z); return ret; }
//
//const XMFLOAT3 operator + (const XMFLOAT3& lhv, const XMFLOAT3& rhv);
//const XMFLOAT3 operator - (const XMFLOAT3& lhv, const XMFLOAT3& rhv);
//const XMFLOAT3 operator * (const XMFLOAT3& lhv, float rhv);
//const XMFLOAT3 operator / (const XMFLOAT3& lhv, float rhv);
//
//bool const operator == (const XMFLOAT3& lhv, const XMFLOAT3& rhv);
//bool const operator != (const XMFLOAT3& lhv, const XMFLOAT3& rhv);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// edit by ChenYuezong 2018/05/25
// operator " = [] () -> " must be defined as memeber operator

inline XMFLOAT3& operator += (XMFLOAT3& lhv, const XMFLOAT3& rhv) { lhv.x += rhv.x; lhv.y += rhv.y; lhv.z += rhv.z; return lhv; }
inline XMFLOAT3& operator -= (XMFLOAT3& lhv, const XMFLOAT3& rhv) { lhv.x -= rhv.x; lhv.y -= rhv.y; lhv.z -= rhv.z; return lhv; }
inline XMFLOAT3& operator *= (XMFLOAT3& lhv, float rhv) { lhv.x *= rhv; lhv.y *= rhv; lhv.z *= rhv; return lhv; }
inline XMFLOAT3& operator /= (XMFLOAT3& lhv, float rhv) { lhv.x /= rhv; lhv.y /= rhv; lhv.z /= rhv; return lhv; }

//inline XMFLOAT3 operator + () const { XMFLOAT3 ret(x, y, z); return ret; }
//inline XMFLOAT3 operator - () const { XMFLOAT3 ret(-x, -y, -z); return ret; }

inline const XMFLOAT3 operator + (const XMFLOAT3& lhv, const XMFLOAT3& rhv) { return XMFLOAT3(lhv.x + rhv.x, lhv.y + rhv.y, lhv.z + rhv.z); }
inline const XMFLOAT3 operator - (const XMFLOAT3& lhv, const XMFLOAT3& rhv) { return XMFLOAT3(lhv.x - rhv.x, lhv.y - rhv.y, lhv.z - rhv.z); }
inline const XMFLOAT3 operator * (const XMFLOAT3& lhv, float rhv) { XMFLOAT3 ret(lhv.x*rhv, lhv.y*rhv, lhv.z*rhv); return ret; }
inline const XMFLOAT3 operator / (const XMFLOAT3& lhv, float rhv) { XMFLOAT3 ret(lhv.x / rhv, lhv.y / rhv, lhv.z / rhv); return ret; }

inline bool const operator == (const XMFLOAT3& lhv, const XMFLOAT3& rhv)
{
	return (fabsf(lhv.x - rhv.x) < FLT_EPSILON) && (fabsf(lhv.y - rhv.y) < FLT_EPSILON) && (fabsf(lhv.z - rhv.z) < FLT_EPSILON);
}
inline bool const operator != (const XMFLOAT3& lhv, const XMFLOAT3& rhv)
{
	return (fabsf(lhv.x - rhv.x) > FLT_EPSILON) || (fabsf(lhv.y - rhv.y) > FLT_EPSILON) || (fabsf(lhv.z - rhv.z) > FLT_EPSILON);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Transform
{
	XMFLOAT3 position;
	XMFLOAT3 scaling;
	XMFLOAT3 rotationAxis;
	FLOAT	 angle;
	// TODO : Upgrade EulerAngles to Quaternions
	XMFLOAT3 eulerAngle;
	Transform() :position(0, 0, 0), scaling(1, 1, 1), rotationAxis(0, 1, 0), angle(0), eulerAngle(0, 0, 0) {};
	void clear() {
		position = { 0.0f, 0.0f, 0.0f };
		scaling = { 1.0f, 1.0f, 1.0f };
		rotationAxis = { 0.0f, 1.0f, 0.0f };
		angle = 0.0f;
		eulerAngle = { 0.0f, 0.0f, 0.0f };
	};
	static inline Transform initialValue() {
		Transform clearedValue;
		clearedValue.clear();
		return clearedValue;
	};
};

struct CameraData
{
	XMVECTOR eyePosition;
	XMVECTOR focusPosition;
	XMVECTOR upDirection;
	D3D11_VIEWPORT viewPort;
	CameraData() :eyePosition({ 1,1,-1,0 }), focusPosition({ 0,0,0,0 }), upDirection({ 0,1,0,0 }) {
		viewPort.Width = SCREEN_WIDTH;
		viewPort.Height = SCREEN_HEIGHT;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
	};
	void clear() {
		eyePosition = { 1, 1, -1,0 };
		focusPosition = { 0, 0, 0 ,0 };
		upDirection = { 0, 1, 0, 0 };
	};
	void toNDC() {
		eyePosition /= SCREEN_WIDTH;
		focusPosition /= SCREEN_WIDTH;
	};
};

extern CameraData e_mainCamera;

extern std::unique_ptr<DirectX::Keyboard> e_pKeyboard;
extern DirectX::Keyboard::State KEY_BOARD;
extern DirectX::Keyboard::KeyboardStateTracker KEY_TRACKER;

extern std::unique_ptr<DirectX::GamePad> e_pGamePad;
extern DirectX::GamePad::State GAME_PAD;
extern DirectX::GamePad::ButtonStateTracker PAD_TRACKER;

extern bool e_isAnyKeyDown;

inline void setCamera(const CameraData &cameraData)
{
	e_mainCamera = cameraData;
};


typedef UINT UINTCOLOR;

extern int e_renderTargetWidth;
extern int e_renderTargetHeight;

inline void setRenderTargetWH(int width, int height)
{
	e_renderTargetWidth = width;
	e_renderTargetHeight = height;
	//e_renderTargetWidth = SCREEN_WIDTH;
	//e_renderTargetHeight = SCREEN_HEIGHT;
}

// 
inline XMFLOAT3 toNDC_RT(XMFLOAT3 coord, bool doProjection = false)
{
	float x, y, z = 0;

	// Different Coordinate System
	if (doProjection)
	{	// 2D with projection
		x = coord.x / (float)SCREEN_WIDTH;
		y = coord.y / (float)SCREEN_WIDTH;
		z = coord.z / (float)SCREEN_WIDTH;
	}
	else
	{	// 2D without projection
		x = 2.0f*coord.x / e_renderTargetWidth - 1.0f;
		y = 1.0f - 2.0f*coord.y / e_renderTargetHeight;
	}

	return XMFLOAT3(x, y, z);
}

inline XMFLOAT4 toNDColor(UINTCOLOR inColor)
{
	float red, green, blue, alph;
	/*red		= (inColor >> 24 & 0xFF) / 255.0f;
	green	= (inColor >> 16 & 0xFF) / 255.0f;
	blue	= (inColor >> 8 & 0xFF) / 255.0f;
	alph	= (inColor & 0xFF) / 255.0f;*/

	/*red = inColor >> 24 & 0xFF;
	green = inColor >> 16 & 0xFF;
	blue = inColor >> 8 & 0xFF;
	alph = inColor & 0xFF;
	red /= 255.0f;
	green /= 255.0f;
	blue /= 255.0f;
	alph /= 255.0f;*/

	red = (inColor & 0xFF000000) / (float)0xFF000000;
	green = (inColor & 0xFF0000) / (float)0xFF0000;
	blue = (inColor & 0xFF00) / (float)0xFF00;
	alph = (inColor & 0xFF) / (float)0xFF;

	return XMFLOAT4(red, green, blue, alph);
}



#endif // ! _3DGPSYSTEM_H_
