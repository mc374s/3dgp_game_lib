#include "directxtk.h"
#include <algorithm>

using namespace DirectX;

ID3D11InputLayout*										DXTK::pBatchInputLayout = nullptr;
std::unique_ptr<CommonStates>                           DXTK::States;
std::unique_ptr<BasicEffect>                            DXTK::BatchEffect;
std::unique_ptr<GeometricPrimitive>                     DXTK::Cube;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>>    DXTK::Batch;

HRESULT DXTK::CreateDirectXTKObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	HRESULT hr = S_OK;
	Batch.reset(new PrimitiveBatch<VertexPositionColor>(pDeviceContext));
	States.reset(new CommonStates(pDevice));
	BatchEffect.reset(new BasicEffect(pDevice));
	BatchEffect->SetVertexColorEnabled(true);

	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		hr = pDevice->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&pBatchInputLayout);
		if (FAILED(hr))
			return hr;
	}

	Cube = GeometricPrimitive::CreateCube(pDeviceContext, 1, false);

	return S_OK;
}

void DXTK::CleanupDirectXTKObject() 
{
	if (pBatchInputLayout) pBatchInputLayout->Release();
}

void XM_CALLCONV DXTK::DrawGrid(ID3D11DeviceContext* pDeviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	BatchEffect->SetWorld(world);
	BatchEffect->SetView(view);
	BatchEffect->SetProjection(projection);
	
	BatchEffect->Apply(pDeviceContext);

	pDeviceContext->IASetInputLayout(pBatchInputLayout);
	//pDeviceContext->OMSetDepthStencilState(States->DepthDefault(), 1);

	Batch->Begin();

	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);
	float fPercent;
	XMVECTOR vScale;
	VertexPositionColor v1, v2;
	for (size_t i = 0; i <= xdivs; ++i)
	{
		fPercent = float(i) / float(xdivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		vScale = XMVectorScale(xAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		XMStoreFloat3(&v1.position, XMVectorSubtract(vScale, yAxis));
		XMStoreFloat4(&v1.color, color);
		XMStoreFloat3(&v2.position, XMVectorAdd(vScale, yAxis));
		XMStoreFloat4(&v2.color, color);
		//VertexPositionColor v1 = VertexPositionColor(XMVectorSubtract(vScale, yAxis), color);
		//VertexPositionColor v2 = VertexPositionColor(XMVectorAdd(vScale, yAxis), color);
		Batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; ++i)
	{
		fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		XMStoreFloat3(&v1.position, XMVectorSubtract(vScale, xAxis));
		XMStoreFloat4(&v1.color, color);
		XMStoreFloat3(&v2.position, XMVectorAdd(vScale, xAxis));
		XMStoreFloat4(&v2.color, color);
		//VertexPositionColor v1 = VertexPositionColor(XMVectorSubtract(vScale, xAxis), color);
		//VertexPositionColor v2 = VertexPositionColor(XMVectorAdd(vScale, xAxis), color);
		Batch->DrawLine(v1, v2);
	}

	Batch->End();

}

void XM_CALLCONV DXTK::DrawAABB(ID3D11DeviceContext* pDeviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, FXMVECTOR minPos, FXMVECTOR maxPos, GXMVECTOR color)
{
	BatchEffect->SetWorld(world);
	BatchEffect->SetView(view);
	BatchEffect->SetProjection(projection);

	BatchEffect->Apply(pDeviceContext);

	pDeviceContext->IASetInputLayout(pBatchInputLayout);
	//pDeviceContext->OMSetDepthStencilState(States->DepthDefault(), 1);

	Batch->Begin();

	static XMFLOAT3 min, max;
	XMStoreFloat3(&min, minPos);
	XMStoreFloat3(&max, maxPos);

	static XMFLOAT4 colorF;
	XMStoreFloat4(&colorF, color);

	static const int LINE_NUM_DOUBLE = 24;

	VertexPositionColor vertices[LINE_NUM_DOUBLE] = {
		{ XMFLOAT3(min.x, min.y, min.z), colorF },
		{ XMFLOAT3(max.x, min.y, min.z), colorF },
		{ XMFLOAT3(max.x, min.y, min.z), colorF },
		{ XMFLOAT3(max.x, min.y, max.z), colorF },
		{ XMFLOAT3(max.x, min.y, max.z), colorF },
		{ XMFLOAT3(min.x, min.y, max.z), colorF },
		{ XMFLOAT3(min.x, min.y, max.z), colorF },
		{ XMFLOAT3(min.x, min.y, min.z), colorF },

		{ XMFLOAT3(min.x, max.y, min.z), colorF },
		{ XMFLOAT3(max.x, max.y, min.z), colorF },
		{ XMFLOAT3(max.x, max.y, min.z), colorF },
		{ XMFLOAT3(max.x, max.y, max.z), colorF },
		{ XMFLOAT3(max.x, max.y, max.z), colorF },
		{ XMFLOAT3(min.x, max.y, max.z), colorF },
		{ XMFLOAT3(min.x, max.y, max.z), colorF },
		{ XMFLOAT3(min.x, max.y, min.z), colorF },

		{ XMFLOAT3(min.x, min.y, min.z), colorF },
		{ XMFLOAT3(min.x, max.y, min.z), colorF },
		{ XMFLOAT3(max.x, min.y, min.z), colorF },
		{ XMFLOAT3(max.x, max.y, min.z), colorF },
		{ XMFLOAT3(max.x, min.y, max.z), colorF },
		{ XMFLOAT3(max.x, max.y, max.z), colorF },
		{ XMFLOAT3(min.x, min.y, max.z), colorF },
		{ XMFLOAT3(min.x, max.y, max.z), colorF }
	};

	Batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertices, LINE_NUM_DOUBLE);

	Batch->End();
}

void XM_CALLCONV DXTK::DrawSphere(ID3D11DeviceContext* pDeviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, FXMVECTOR centerPos, float radius, GXMVECTOR color)
{
	BatchEffect->SetWorld(world);
	BatchEffect->SetView(view);
	BatchEffect->SetProjection(projection);

	BatchEffect->Apply(pDeviceContext);

	pDeviceContext->IASetInputLayout(pBatchInputLayout);
	//pDeviceContext->OMSetDepthStencilState(States->DepthDefault(), 1);

	Batch->Begin();

	static XMFLOAT4 colorF;
	XMStoreFloat4(&colorF, color);

	static const int CIRCLE_NUM = 3;
	static const int POINT_NUM = 36;

	VertexPositionColor vertices[CIRCLE_NUM][POINT_NUM];

	for (int circle = 0; circle < CIRCLE_NUM; ++circle)
	{
		for (int point = 0; point < POINT_NUM; ++point)
		{
			switch (circle)
			{
			case 0: 
				XMStoreFloat3(&vertices[0][point].position, radius * XMLoadFloat3(&XMFLOAT3(sinf(360.0f / POINT_NUM * 0.01745f * point), cosf(360.0f / POINT_NUM * 0.01745f * point), 0)) + centerPos);
				break;
			case 1:
				XMStoreFloat3(&vertices[1][point].position, radius * XMLoadFloat3(&XMFLOAT3(sinf(360.0f / POINT_NUM * 0.01745f * point), 0, cosf(360.0f / POINT_NUM * 0.01745f * point))) + centerPos);
				break;
			case 2:
				XMStoreFloat3(&vertices[2][point].position, radius * XMLoadFloat3(&XMFLOAT3(0, sinf(360.0f / POINT_NUM * 0.01745f * point), cosf(360.0f / POINT_NUM * 0.01745f * point))) + centerPos);
				break;
			}
			vertices[circle][point].color = colorF;
		}

		Batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, vertices[circle], POINT_NUM);
	}

	Batch->End();
}