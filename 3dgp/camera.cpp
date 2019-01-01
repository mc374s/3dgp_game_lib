#include "camera.h"

using namespace DirectX;

using namespace GLC;

Camera GLC::mainCamera;

Camera::Camera():
	eyePosition({ 1,1,-1,0 }), 
	focusPosition({ 0,0,0,0 }), 
	upDirection({ 0,1,0,0 }),
	FovAngleY(XM_PIDIV4)
{
	viewPort.Width = 1280;
	viewPort.Height = 720;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
}

void Camera::Clear()
{
	eyePosition = { 1, 1, -1,0 };
	focusPosition = { 0, 0, 0 ,0 };
	upDirection = { 0, 1, 0, 0 };
}

void Camera::Update()
{
	view = XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	projection = XMMatrixPerspectiveFovLH(FovAngleY, viewPort.Width / viewPort.Height, 0.01f, 100.0f);
}