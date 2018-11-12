#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	XMVECTOR eyePosition;
	XMVECTOR focusPosition;
	XMVECTOR upDirection;
	D3D11_VIEWPORT viewPort;

	Camera() :eyePosition({ 1,1,-1,0 }), focusPosition({ 0,0,0,0 }), upDirection({ 0,1,0,0 }) {
		viewPort.Width = SCREEN_WIDTH;
		viewPort.Height = SCREEN_HEIGHT;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
	};

	void Clear() {
		eyePosition = { 1, 1, -1,0 };
		focusPosition = { 0, 0, 0 ,0 };
		upDirection = { 0, 1, 0, 0 };
	};

	XMMATRIX View;

	static Camera mainCamera;
};

#endif // !_CAMERA_H_

