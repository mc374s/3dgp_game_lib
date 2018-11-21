#include "3dgp_math.h"

using namespace DirectX;

bool Collision::HitJudgement(const Collision* other)
{
	bool isHitted = false;
	const Collision* A = this;
	const Collision* B = other;
	switch (type | other->type)
	{
	case _SPHERE | _SPHERE:
		isHitted = SphereHitSphere(((Sphere*)this)->center, ((Sphere*)this)->radius, ((Sphere*)other)->center, ((Sphere*)other)->radius);
		break;
	case _SPHERE | _AABB:
		if (type == _AABB)
		{
			A = other;
			B = this;
		}
		isHitted = SphereHitAABB(((Sphere*)A)->center, ((Sphere*)A)->radius, ((AABB*)B)->minPos, ((AABB*)B)->maxPos);
		break;
	case _AABB | _AABB:
		isHitted = AABBHitAABB(((AABB*)this)->minPos, ((AABB*)this)->maxPos, ((AABB*)other)->minPos, ((AABB*)other)->maxPos);
		break;
	default:
		break;
	}
	
	return isHitted;
}

bool XM_CALLCONV SphereHitSphere(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR centerB, float radiusB)
{
	if (XMVectorGetX(XMVector3Length(centerB - centerA)) > radiusA + radiusB)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool XM_CALLCONV AABBHitAABB(DirectX::FXMVECTOR minPosA, DirectX::FXMVECTOR maxPosA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB)
{
	XMFLOAT3 minA, maxA, minB, maxB;
	XMStoreFloat3(&minA, minPosA);
	XMStoreFloat3(&maxA, maxPosA);
	XMStoreFloat3(&minB, minPosB);
	XMStoreFloat3(&maxB, maxPosB);
	if (minA.x > maxB.x) return false;
	if (maxA.x < minB.x) return false;
	if (minA.y > maxB.y) return false;
	if (maxA.y < minB.y) return false;
	if (minA.z > maxB.z) return false;
	if (maxA.z < minB.z) return false;
	return true;
}

bool XM_CALLCONV SphereHitAABB(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB)
{
	XMFLOAT3 center, minB, maxB;
	XMStoreFloat3(&center, centerA);
	XMStoreFloat3(&minB, minPosB);
	XMStoreFloat3(&maxB, maxPosB);

	XMFLOAT3 closestPoint(0, 0, 0); // Should be Setting to AABB's Surface 
	closestPoint = center;
	if (closestPoint.x < minB.x) {
		closestPoint.x = minB.x;
	}
	if (closestPoint.x > maxB.x) {
		closestPoint.x = maxB.x;
	}
	if (closestPoint.y < minB.y) {
		closestPoint.y = minB.y;
	}
	if (closestPoint.y > maxB.y) {
		closestPoint.y = maxB.y;
	}
	if (closestPoint.z < minB.z) {
		closestPoint.z = minB.z;
	}
	if (closestPoint.z > maxB.z) {
		closestPoint.z = maxB.z;
	}
	// Distance of closestPoint and Sphere's center Bigger then Shpere's radius, not hitted
	if (XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&closestPoint), centerA))) > radiusA) {
		return false;
	}
	return true;
}