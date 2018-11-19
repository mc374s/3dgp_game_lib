#ifndef _3DGP_MATH_
#define _3DGP_MATH_

#include <DirectXMath.h>

typedef unsigned int UINTCOLOR;

inline DirectX::XMVECTOR XMConvertUIntToColor(UINTCOLOR inColor)
{
	DirectX::XMFLOAT4 rgba;
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

	rgba.x = (inColor & 0xFF000000) / (float)0xFF000000;
	rgba.y = (inColor & 0xFF0000) / (float)0xFF0000;
	rgba.z = (inColor & 0xFF00) / (float)0xFF00;
	rgba.w = (inColor & 0xFF) / (float)0xFF;

	return DirectX::XMLoadFloat4(&rgba);
}

struct AABB 
{
	DirectX::XMFLOAT3 minPos;
	DirectX::XMFLOAT3 maxPos;
	AABB(DirectX::XMFLOAT3 minPos, DirectX::XMFLOAT3 manPos) :minPos(minPos), maxPos(maxPos) {};
};



#endif // !_3DGP_MATH_
