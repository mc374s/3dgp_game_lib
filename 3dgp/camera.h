#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3d11.h>
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
		viewPort.Width = 1280;
		viewPort.Height = 720;
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

