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

struct Collision
{
	enum TYPE {
		_SPHERE = 1 << 0,
		_PLANE	= 1 << 1,
		_AABB	= 1 << 2,
	};
	int type = _SPHERE;

	virtual bool HitJudgement(const Collision* other);
};

struct Sphere :public Collision
{
	DirectX::XMVECTOR center;
	float radius;
	Sphere() { type = _SPHERE; };
	Sphere(DirectX::FXMVECTOR center, float radius) : center(center), radius(radius) { type = _SPHERE; };
};

struct Plane :public Collision
{
	DirectX::XMVECTOR direction;
	float disToOrigin;
	Plane();
	Plane(DirectX::FXMVECTOR direction, float disToOrigin) :direction(direction), disToOrigin(disToOrigin) { type = _PLANE; };
};

struct AABB :public Collision
{
	DirectX::XMVECTOR minPos;
	DirectX::XMVECTOR maxPos;
	AABB() { type = _AABB; };
	AABB(DirectX::FXMVECTOR minPos, DirectX::FXMVECTOR maxPos) :minPos(minPos), maxPos(maxPos) { type = _AABB; };
};

bool XM_CALLCONV SphereHitSphere(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR centerB, float radiusB);
bool XM_CALLCONV AABBHitAABB(DirectX::FXMVECTOR minPosA, DirectX::FXMVECTOR maxPosA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB);

bool XM_CALLCONV SphereHitAABB(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB);



#endif // !_3DGP_MATH_
