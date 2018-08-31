#include "render_target.h"

#include <iostream>
//#define _CRT_SECURE_NO_WARNINGS

using namespace MyResourcesManager;
using namespace DirectX;

bool RenderTarget::initialize(ID3D11Device* a_pDevice)
{
	m_vertexCount = 4 /*sizeof(vertices) / sizeof(vertex)*/;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*//*D3D11_USAGE_DEFAULT*/D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertex) * m_vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE/*0*/;

	// Create vertex buffer
	hr = a_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Initialize m_pVertexBuffer failed", 0, 0);
		return false;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	loadVertexShader(a_pDevice, "3dgp/render_target_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &m_pVertexShader, &m_pInputLayout);

	loadPixelShader(a_pDevice, "3dgp/render_target_ps.cso", &m_pPixelShader);

	// create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;

	hr = a_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateCullBack);
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	hr = a_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateCullFront);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Initialize hr failed", 0, 0);
		return false;
	}

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = a_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create DepthStencilState failed", L"RenderTarget::Initialize", 0);
		return false;
	}

	// Create constant buffer
	//ZeroMemory(&VSConstantData, sizeof(VS_CONSTANT_BUFFER));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	//subResourceData.pSysMem = &VSConstantData;
	bufferDesc.ByteWidth = sizeof(PROJECTION_CBUFFER);

	hr = a_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pVSProjectionCBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create m_pConstantBuffer failed", L"RenderTarget::Initialize()", 0);
		return false;
	}

	return true;
}

RenderTarget::RenderTarget(ID3D11Device* a_pDevice, int a_renderWidth, int a_renderHeight)
{
	initialize(a_pDevice);
	//loadShaderResourceView(a_pDevice, a_pFilename, &resource, &m_pShaderResourceView);

	//// TEXTURE2D_DESC Initialize
	//m_Texture2dDesc.MipLevels = 1;
	//m_Texture2dDesc.ArraySize = 1;
	//m_Texture2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//m_Texture2dDesc.SampleDesc.Count = 1;
	//m_Texture2dDesc.SampleDesc.Quality = 0;
	//m_Texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	//m_Texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//m_Texture2dDesc.CPUAccessFlags = 0;
	//m_Texture2dDesc.MiscFlags = 0;

	//if (resource == NULL)
	//{
	//	MessageBox(0, L"sprite: Create Recource failed", 0, 0);
	//	return;
	//}
	//resource->QueryInterface(&texture2d);
	//texture2d->GetDesc(&m_Texture2dDesc);

	///////////////////////////////////////////
	// Render to Texture test
	ZeroMemory(&m_renderTargetTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	m_renderTargetTextureDesc.Width = a_renderWidth;
	m_renderTargetTextureDesc.Height = a_renderHeight;
	m_renderTargetTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_renderTargetTextureDesc.Format = /*DXGI_FORMAT_R32G32B32A32_FLOAT*/DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_D24_UNORM_S8_UINT*/;
	m_renderTargetTextureDesc.SampleDesc.Count = 1;
	m_renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	m_renderTargetTextureDesc.CPUAccessFlags = 0;
	m_renderTargetTextureDesc.MiscFlags = 0;
	m_renderTargetTextureDesc.MipLevels = 1;
	m_renderTargetTextureDesc.ArraySize = 1;

	hr = a_pDevice->CreateTexture2D(&m_renderTargetTextureDesc, NULL, &m_pRenderTargetTexture);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create Texture2D failed", 0, 0);
		exit(-1);
	}
	//m_pRenderTargetTexture->QueryInterface(&m_pRenderSurface);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = m_renderTargetTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = a_pDevice->CreateRenderTargetView(m_pRenderTargetTexture, &renderTargetViewDesc, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create RenderTargetView failed", 0, 0);
		exit(-1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = m_renderTargetTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = a_pDevice->CreateShaderResourceView(m_pRenderTargetTexture, &shaderResourceViewDesc, &m_pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create CreateShaderResourceView failed", 0, 0);
		exit(-1);
	}

	///////////////////////////////////////////

}


RenderTarget::~RenderTarget()
{

	SAFE_RELEASE(m_pRasterizerStateCullBack);
	SAFE_RELEASE(m_pRasterizerStateCullFront);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVSProjectionCBuffer);
	
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pRenderTargetTexture);
	SAFE_RELEASE(m_pRenderTargetView);


	SAFE_RELEASE(m_pShaderResourceView);
	releasePixelShader(m_pPixelShader);
	releaseVertexShader(m_pVertexShader, m_pInputLayout);


}

void RenderTarget::render(ID3D11DeviceContext* a_pDeviceContext)
{

	UINT pStrides = sizeof(vertex);
	UINT pOffsets = 0;
	a_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &pStrides, &pOffsets);
	a_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*//*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	a_pDeviceContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
	a_pDeviceContext->IASetInputLayout(m_pInputLayout);
	a_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	a_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	a_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	a_pDeviceContext->Draw(m_vertexCount, 0);

	float ClearColor[4] = { 128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 0.0f };
	a_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	a_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	setRenderTargetWH(m_virtualWidth, m_virtualHeight);

	e_camera.viewPort.Width = m_virtualWidth;
	e_camera.viewPort.Height = m_virtualHeight;
	a_pDeviceContext->RSSetViewports(1, &e_camera.viewPort);
}

void RenderTarget::render(ID3D11DeviceContext* a_pDeviceContext, vertex a_pCoordNDC[])
{

	D3D11_MAPPED_SUBRESOURCE mappedSubRec;
	hr = a_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD/*D3D11_MAP_WRITE_NO_OVERWRITE*/, 0, &mappedSubRec);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Render Map failed", 0, 0);
		return;
	}
	memcpy(mappedSubRec.pData, a_pCoordNDC, sizeof(vertex)*m_vertexCount);
	a_pDeviceContext->Unmap(m_pVertexBuffer, 0);

	render(a_pDeviceContext);

}

void RenderTarget::render(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX, float a_srcY, float a_srcWidth, float a_srcHeight, float a_rotateAngle, UINTCOLOR a_blendColor, bool a_doReflection)
{
	if ((int)a_srcWidth == 0 || (int)a_srcHeight == 0)
	{
		a_srcWidth = a_drawWidth;
		a_srcHeight = a_drawHeight;
	}

	// Caculate 2D texture coordinate and update vertics buffer
	XMFLOAT4 colorNDC;
	colorNDC = toNDColor(a_blendColor);
	vertex vertices[] = {
		{
			XMFLOAT3(a_drawX, a_drawY, 0),								XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(a_srcX, a_srcY)
		},
		{
			XMFLOAT3(a_drawX + a_drawWidth, a_drawY, 0),				XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(a_srcX + a_srcWidth, a_srcY)
		},
		{
			XMFLOAT3(a_drawX, a_drawY + a_drawHeight, 0),				XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(a_srcX , a_srcY + a_srcHeight)
		},
		{
			XMFLOAT3(a_drawX + a_drawWidth, a_drawY + a_drawHeight, 0),	XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(a_srcX + a_srcWidth, a_srcY + a_srcHeight)
		},
	};



	XMFLOAT3 center(a_drawX + a_drawWidth / 2, a_drawY + a_drawHeight / 2, 0);

	// Rotation And Change to NDC coordinate
	float angleRadian = a_rotateAngle * 0.01745/*(M_PI / 180,0f)*/;
	for (unsigned int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = rotationZ(vertices[i].position, angleRadian, center);
		vertices[i].position = toNDC(vertices[i].position);
		vertices[i].texcoord = toNDC_UV(vertices[i].texcoord);
		vertices[i].normal = { 0,0,-1 };
		if (a_doReflection)
		{
			vertices[i].normal.x = -vertices[i].normal.x;
			vertices[i].normal.y = -vertices[i].normal.y;
			vertices[i].normal.z = -vertices[i].normal.z;
		}
	}
	if (a_doReflection)
	{
		static XMFLOAT2 swapTemp;
		swapTemp = vertices[0].texcoord;
		vertices[0].texcoord = vertices[1].texcoord;
		vertices[1].texcoord = swapTemp;
		swapTemp = vertices[2].texcoord;
		vertices[2].texcoord = vertices[3].texcoord;
		vertices[3].texcoord = swapTemp;
		a_pDeviceContext->RSSetState(m_pRasterizerStateCullFront);
	}
	else
	{
		a_pDeviceContext->RSSetState(m_pRasterizerStateCullBack);
	}

	render(a_pDeviceContext, vertices);
}

void RenderTarget::setProjection(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const CUSTOM3D* a_pCustom3D)
{
	if (a_pCustom3D == nullptr)
	{
		CUSTOM3D init;
		a_pCustom3D = &init;
	}
	static XMFLOAT3 position, rotationAxis;
	position = toNDC(a_pCustom3D->position);
	rotationAxis = toNDC(a_pCustom3D->rotationAxis);
	static XMMATRIX S, R, T, W, V, P, WVP;
	S = R = T = W = V = P = WVP = DirectX::XMMatrixIdentity();
	//S = DirectX::XMMatrixScaling(a_pCustom3D->scaling.x, a_pCustom3D->scaling.y, a_pCustom3D->scaling.z);
	////R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0), a_pCustom3D->a_rotateAngle*0.01745329251);
	//R = DirectX::XMMatrixRotationRollPitchYaw(a_pCustom3D->angleYawPitchRoll.y*0.01745, a_pCustom3D->angleYawPitchRoll.x*0.01745, a_pCustom3D->angleYawPitchRoll.z*0.01745);
	//T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//W = S*R*T;
	//R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0), _custom3D->angle*0.01745329251);
	R = DirectX::XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), a_pCustom3D->angleYawPitchRoll.x*0.01745)
		*DirectX::XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), a_pCustom3D->angleYawPitchRoll.y*0.01745)
		*DirectX::XMMatrixRotationAxis(XMVectorSet(0, 0, 1, 0), a_pCustom3D->angleYawPitchRoll.z*0.01745);
	//R = DX::XMMatrixRotationRollPitchYaw(a_pCustom3D->angleYawPitchRoll.y*0.01745, a_pCustom3D->angleYawPitchRoll.x*0.01745, a_pCustom3D->angleYawPitchRoll.z*0.01745);
	S = DirectX::XMMatrixScaling(a_pCustom3D->scaling.x, a_pCustom3D->scaling.y, a_pCustom3D->scaling.z);
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	W = T*R*S;

	//e_camera.clear();
	V = DirectX::XMMatrixLookAtLH(e_camera.eyePosition, e_camera.focusPosition, e_camera.upDirection);
	P = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
	WVP = W*V*P;
	/*W = DX::XMMatrixTranspose(W);
	V = DX::XMMatrixTranspose(V);
	P = DX::XMMatrixTranspose(P);*/

	PROJECTION_CBUFFER updateCbuffer;
	updateCbuffer.world = W;
	updateCbuffer.view = V;
	updateCbuffer.projection = P;
	updateCbuffer.worldViewProjection = WVP;
	static XMVECTOR lightV;
	lightV = (e_camera.focusPosition - e_camera.eyePosition);
	updateCbuffer.lightDirection = XMFLOAT4(lightV.vector4_f32[0], lightV.vector4_f32[1], lightV.vector4_f32[2], 0.0f);
	updateCbuffer.materialColor = XMFLOAT4(0.8, 0.8, 0, 1);

	a_pDeviceContext->UpdateSubresource(m_pVSProjectionCBuffer, 0, NULL, &updateCbuffer, 0, 0);
	a_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pVSProjectionCBuffer);
}

void RenderTarget::render3D(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX, float a_srcY, float a_srcWidth, float a_srcHeight, float a_rotateAngle, UINTCOLOR a_blendColor, const CUSTOM3D* a_pCustom3D, bool a_doReflection)
{
	// 
	m_virtualWidth = a_srcWidth;
	m_virtualHeight = a_srcHeight;
	setProjection(a_pDeviceContext, XMFLOAT3(0, 0, 0), a_pCustom3D);
	render(a_pDeviceContext, a_drawX, a_drawY, a_drawWidth, a_drawHeight, a_srcX, a_srcY, a_srcWidth, a_srcHeight, a_rotateAngle, a_blendColor, a_doReflection);
}

XMFLOAT3 RenderTarget::toNDC(float a_screenX, float a_screenY)
{
	float x, y;
	x = 2.0f*a_screenX / SCREEN_WIDTH - 1.0f;
	y = 1.0f - 2.0f*a_screenY / SCREEN_HEIGHT;
	return XMFLOAT3(x, y, 0);
}

XMFLOAT3 RenderTarget::toNDC(XMFLOAT3 a_coord)
{
	float x, y, z;
	// 2D without projection
	/*x = 2.0f*_coord.x / SCREEN_WIDTH - 1.0f;
	y = 1.0f - 2.0f*_coord.y / SCREEN_WIDTH;*/

	// 2D with projection
	x = a_coord.x / (float)SCREEN_WIDTH;
	y = -a_coord.y / (float)SCREEN_WIDTH;
	z = a_coord.z / (float)SCREEN_WIDTH;
	return XMFLOAT3(x, y, z);
}

// Screen coordinate to UV NDC coordinate
XMFLOAT2 RenderTarget::toNDC_UV(XMFLOAT2 a_coord)
{
	float x, y;
	float imgWidth, imgHeight;
	imgWidth = m_renderTargetTextureDesc.Width;
	imgHeight = m_renderTargetTextureDesc.Height;

	x = a_coord.x / imgWidth;
	y = a_coord.y / imgHeight;
	return XMFLOAT2(x, y);
}

XMFLOAT3 RenderTarget::rotationZ(XMFLOAT3 a_coord, float a_rotateAngle, XMFLOAT3 a_centerCoord)
{
	XMFLOAT3 P, Pr;
	P.x = a_coord.x - a_centerCoord.x;
	P.y = a_coord.y - a_centerCoord.y;
	P.z = 0/*_coord.z - _center.z*/;
	// rotation
	Pr.x = P.x*cosf(a_rotateAngle) - P.y*sinf(a_rotateAngle) + a_centerCoord.x;
	Pr.y = P.y*cosf(a_rotateAngle) + P.x*sinf(a_rotateAngle) + a_centerCoord.y;
	Pr.z = 0;
	return Pr;
}


