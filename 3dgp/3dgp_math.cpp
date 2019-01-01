#include "3dgp_math.h"

using namespace DirectX;

HitResult Collision::HitJudgement(const Collision* other)
{
	HitResult hitResult;
	int directionAdjustion = 1;
	const Collision* A = this;
	const Collision* B = other;
	switch (type | other->type)
	{
	case _SPHERE | _SPHERE:
		hitResult = SphereHitSphere(((Sphere*)this)->center, ((Sphere*)this)->radius, ((Sphere*)other)->center, ((Sphere*)other)->radius);
		break;
	case _SPHERE | _AABB:
		if (type == _AABB) {
			A = other;
			B = this;
			directionAdjustion = -1;
		}
		hitResult = SphereHitAABB(((Sphere*)A)->center, ((Sphere*)A)->radius, ((AABB*)B)->minPos, ((AABB*)B)->maxPos);
		break;
	case _AABB | _AABB:
		hitResult = AABBHitAABB(((AABB*)this)->minPos, ((AABB*)this)->maxPos, ((AABB*)other)->minPos, ((AABB*)other)->maxPos);
		break;
	default:
		break;
	}

	hitResult.direction *= directionAdjustion;
	return hitResult;
}

HitResult XM_CALLCONV SphereHitSphere(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR centerB, float radiusB)
{
	HitResult hitResult;

	if (XMVectorGetX(XMVector3Length(centerB - centerA)) > radiusA + radiusB) {
		hitResult.isHitted = false;
	}
	else {
		hitResult.isHitted = true;
	}
	hitResult.direction = XMVector3Normalize(XMVectorSubtract(centerB, centerA));
	hitResult.closestPoint = centerB - hitResult.direction*radiusB;

	return hitResult;
}

HitResult XM_CALLCONV AABBHitAABB(DirectX::FXMVECTOR minPosA, DirectX::FXMVECTOR maxPosA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB)
{
	HitResult hitResult;
	XMFLOAT3 minA, maxA, minB, maxB;
	XMStoreFloat3(&minA, minPosA);
	XMStoreFloat3(&maxA, maxPosA);
	XMStoreFloat3(&minB, minPosB);
	XMStoreFloat3(&maxB, maxPosB);

	XMVECTOR centerA = (minPosA + maxPosA)*0.5f;

	XMFLOAT3 closestPoint(0, 0, 0); // Should be Setting to AABB's Surface 
	XMStoreFloat3(&closestPoint, centerA);
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
	hitResult.closestPoint = XMLoadFloat3(&closestPoint);
	hitResult.direction = XMVector3Normalize(XMVectorSubtract(hitResult.closestPoint, centerA));

	hitResult.isHitted = false;
	if (minA.x > maxB.x) {
		return hitResult;
	}
	if (maxA.x < minB.x) {
		return hitResult;
	}
	if (minA.y > maxB.y) {
		return hitResult;
	}
	if (maxA.y < minB.y) {
		return hitResult;
	}
	if (minA.z > maxB.z) {
		return hitResult;
	}
	if (maxA.z < minB.z) {
		return hitResult;
	}
	hitResult.isHitted = true;
	return hitResult;
}

HitResult XM_CALLCONV SphereHitAABB(DirectX::FXMVECTOR centerA, float radiusA, DirectX::FXMVECTOR minPosB, DirectX::FXMVECTOR maxPosB)
{
	HitResult hitResult;
	XMFLOAT3 minB, maxB;
	XMStoreFloat3(&minB, minPosB);
	XMStoreFloat3(&maxB, maxPosB);

	hitResult.isHitted = false;

	XMFLOAT3 closestPoint(0, 0, 0); // Should be Setting to AABB's Surface 
	XMStoreFloat3(&closestPoint, centerA);
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

	hitResult.closestPoint = XMLoadFloat3(&closestPoint);
	hitResult.direction = XMVector3Normalize(XMVectorSubtract(hitResult.closestPoint, centerA));


	// Distance of closestPoint and Sphere's center Bigger then Shpere's radius, not hitted
	if (XMVectorGetX(XMVector3Length(XMVectorSubtract(hitResult.closestPoint, centerA))) <= radiusA) {
		hitResult.isHitted = true;
	}
	return hitResult;
}