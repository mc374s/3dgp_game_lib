#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3d11.h>
//#define _XM_NO_INTRINSICS_ 
#include <DirectXMath.h>

namespace GLC{

__declspec(align(16)) class Camera
{
private:
	//DirectX::XMMATRIX view;
	//DirectX::XMMATRIX projection;

public:
	DirectX::XMVECTOR	eyePosition;
	DirectX::XMVECTOR	focusPosition;
	DirectX::XMVECTOR	upDirection;
	D3D11_VIEWPORT		viewPort;
	float				FovAngleY;


	Camera();

	void Clear();

	DirectX::XMMATRIX XM_CALLCONV GetProjection();
	DirectX::XMMATRIX XM_CALLCONV GetView();
};

extern Camera mainCamera;

};

#endif // !_CAMERA_H_

