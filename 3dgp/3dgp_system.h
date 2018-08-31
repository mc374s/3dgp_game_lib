#ifndef  _3DGPSYSTEM_H_
#define  _3DGPSYSTEM_H_


#define _XM_NO_INTRINSICS_

#include <windows.h>
#include <memory>
#include <d3d11.h>
#include <mfplay.h>
#include <tchar.h>
#include <sstream>
#include <assert.h>
#include <tchar.h>

#include <DirectXMath.h>
#include "WICTextureLoader.h"

#include "misc.h"
#include "high_resolution_timer.h"

#include "resources_manager.h"

#define _TEST (0)

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

using namespace DirectX;


typedef UINT UINTCOLOR;

inline XMFLOAT4 toNDCColor(UINTCOLOR a_inColor)
{
	float red, green, blue, alph;
	/*red	= (_color >> 24 & 0xFF) / 255.0f;
	green	= (_color >> 16 & 0xFF) / 255.0f;
	blue	= (_color >> 8 & 0xFF) / 255.0f;
	alph	= (_color & 0xFF) / 255.0f;*/

	/*red = a_inColor >> 24 & 0xFF;
	green = a_inColor >> 16 & 0xFF;
	blue = a_inColor >> 8 & 0xFF;
	alph = a_inColor & 0xFF;
	red /= 255.0f;
	green /= 255.0f;
	blue /= 255.0f;
	alph /= 255.0f;*/

	red		=(a_inColor & 0xFF000000) / (float)0xFF000000;
	green	=(a_inColor & 0xFF0000) / (float)0xFF0000;
	blue	=(a_inColor & 0xFF00) / (float)0xFF00;
	alph	=(a_inColor & 0xFF) / (float)0xFF;

	return XMFLOAT4(red, green, blue, alph);
}

struct CUSTOM3D
{
	XMFLOAT3 position;
	XMFLOAT3 scaling;
	XMFLOAT3 rotationAxis;
	FLOAT	 angle;
	XMFLOAT3 angleYawPitchRoll;
	CUSTOM3D() :position(0, 0, 0), scaling(1, 1, 1), rotationAxis(0, 1, 0), angle(0), angleYawPitchRoll(0, 0, 0) {};
	void clear() {
		position = { 0, 0, 0 };
		scaling = { 1, 1, 1 };
		rotationAxis = { 0, 1, 0 };
		angle = { 0 };
		angleYawPitchRoll = { 0, 0, 0 };
	};
};
struct CameraData
{
	XMVECTOR eyePosition;
	XMVECTOR focusPosition;
	XMVECTOR upDirection;
	CameraData() :eyePosition({ 1,1,-1,0 }), focusPosition({ 0,0,0,0 }), upDirection({ 0,1,0,0 }) {};
	void clear() {
		eyePosition = { 1, 1, -1,0 };
		focusPosition = { 0, 0, 0 ,0 };
		upDirection = { 0, 1, 0, 0 };
	};
};
extern CameraData e_CameraData;

inline void setCamera(const CameraData &_cameraData)
{
	e_CameraData = _cameraData;
};



#endif // ! _3DGPSYSTEM_H_
