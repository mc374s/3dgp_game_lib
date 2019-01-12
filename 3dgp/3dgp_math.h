#ifndef _3DGP_MATH_
#define _3DGP_MATH_


//#define _XM_NO_INTRINSICS_ 
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

struct __declspec(align(16)) HitResult
{
	bool isHitted;
	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR closestPoint;
};

struct __declspec(align(16)) Collision
{
	enum TYPE {
		_SPHERE = 1 << 0,
		_PLANE	= 1 << 1,
		_AABB	= 1 << 2,
		MAX_NUM
	};
	int type = _SPHERE;

	virtual HitResult HitJudgement(const Collision* other);
};

struct __declspec(align(16)) Sphere :public Collision
{
	DirectX::XMVECTOR center;
	float radius;
	Sphere() { type = _SPHERE; };
	Sphere(DirectX::FXMVECTOR center, float radius) : center(center), radius(radius) { type = _SPHERE; };
};

struct __declspec(align(16)) Plane :public Collision
{
	DirectX::XMVECTOR normal;
	float disToOrigin;
	Plane();
	Plane(DirectX::FXMVECTOR normal, float disToOrigin) :normal(normal), disToOrigin(disToOrigin) { type = _PLANE; };
};

struct __declspec(align(16)) AABB :public Collision
{
	DirectX::XMVECTOR minPos;
	DirectX::XMVECTOR maxPos;
	//DirectX::XMVECTOR center;
	//DirectX::XMFLOAT3 size;
	AABB() { type = _AABB; };
	AABB(DirectX::FXMVECTOR minPos, DirectX::FXMVECTOR maxPos) :minPos(minPos), maxPos(maxPos) { 
		type = _AABB;
		//center = (minPos + maxPos)*0.5f;
		//DirectX::XMStoreFloat3(&size, (maxPos - minPos));
	};
	//AABB(DirectX::FXMVECTOR center, DirectX::XMFLOAT3 size) :center(center), size(size) {
	//	type = AABB;
	//	minPos = center - XMLoadFloat3(&size)*0.5f;
	//	maxPos = center + XMLoadFloat3(&size)*0.5f;
	//};
};

HitResult XM_CALLCONV SphereHitSphere(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR centerB, float radiusB);
HitResult XM_CALLCONV AABBHitAABB(DirectX::FXMVECTOR minPosA, DirectX::FXMVECTOR maxPosA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB);

HitResult XM_CALLCONV SphereHitAABB(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB);



#endif // !_3DGP_MATH_
