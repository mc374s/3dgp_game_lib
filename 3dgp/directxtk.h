#ifndef _DIRECTXTK_H_
#define _DIRECTXTK_H_


#include "CommonStates.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "VertexTypes.h"

class DXTK 
{
public:

	static ID3D11InputLayout*														 pBatchInputLayout;
	static std::unique_ptr<DirectX::CommonStates>									 States;
	static std::unique_ptr<DirectX::BasicEffect>									 BatchEffect;
	static std::unique_ptr<DirectX::GeometricPrimitive>								 Cube;
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>    Batch;

	static HRESULT CreateDirectXTKObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	static void CleanupDirectXTKObject();

	// Render a grid using PrimitiveBatch
	static void XM_CALLCONV DrawGrid(ID3D11DeviceContext* pDeviceContext, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, 
		DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
	static void XM_CALLCONV DrawAABB(ID3D11DeviceContext* pDeviceContext, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, 
		DirectX::FXMVECTOR minPos, DirectX::FXMVECTOR maxPos, DirectX::FXMVECTOR color);
	static void XM_CALLCONV DrawSphere(ID3D11DeviceContext* pDeviceContext, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, 
		DirectX::FXMVECTOR centerPos, float radius, DirectX::FXMVECTOR color);

};


#endif // !_PCH_H_
