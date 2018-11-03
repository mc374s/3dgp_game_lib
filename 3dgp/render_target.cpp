#include "render_target.h"

#include <iostream>
//#define _CRT_SECURE_NO_WARNINGS

using namespace RM;
using namespace DirectX;

bool RenderTarget::initialize(ID3D11Device* pDevice)
{
	vertexCount = 4 /*sizeof(vertices) / sizeof(vertex)*/;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*//*D3D11_USAGE_DEFAULT*/D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertex) * vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE/*0*/;

	// Create vertex buffer
	hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Initialize pVertexBuffer failed", 0, 0);
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
	loadVertexShader(pDevice, "Data/Shader/texture_on_3d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);

	loadPixelShader(pDevice, "Data/Shader/texture_on_ps.cso", &pPixelShader);

	// create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;

	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerStateCullBack);
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerStateCullFront);
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

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
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

	hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pVSProjectionCBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pConstantBuffer failed", L"RenderTarget::Initialize()", 0);
		return false;
	}

	return true;
}

RenderTarget::RenderTarget(ID3D11Device* pDevice, int renderWidth, int renderHeight)
{
	initialize(pDevice);
	//loadShaderResourceView(pDevice, pFilename, &resource, &pShaderResourceView);

	//// TEXTURE2D_DESC Initialize
	//Texture2dDesc.MipLevels = 1;
	//Texture2dDesc.ArraySize = 1;
	//Texture2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Texture2dDesc.SampleDesc.Count = 1;
	//Texture2dDesc.SampleDesc.Quality = 0;
	//Texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	//Texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//Texture2dDesc.CPUAccessFlags = 0;
	//Texture2dDesc.MiscFlags = 0;

	//if (resource == NULL)
	//{
	//	MessageBox(0, L"sprite: Create Recource failed", 0, 0);
	//	return;
	//}
	//resource->QueryInterface(&texture2d);
	//texture2d->GetDesc(&Texture2dDesc);

	///////////////////////////////////////////
	// Render to Texture test
	ZeroMemory(&renderTargetTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	renderTargetTextureDesc.Width = renderWidth;
	renderTargetTextureDesc.Height = renderHeight;
	renderTargetTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTargetTextureDesc.Format = /*DXGI_FORMAT_R32G32B32A32_FLOAT*/DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_D24_UNORM_S8_UINT*/;
	renderTargetTextureDesc.SampleDesc.Count = 1;
	renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTargetTextureDesc.CPUAccessFlags = 0;
	renderTargetTextureDesc.MiscFlags = 0;
	renderTargetTextureDesc.MipLevels = 1;
	renderTargetTextureDesc.ArraySize = 1;

	hr = pDevice->CreateTexture2D(&renderTargetTextureDesc, NULL, &pRenderTargetTexture);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create Texture2D failed", 0, 0);
		exit(-1);
	}
	//pRenderTargetTexture->QueryInterface(&pRenderSurface);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = renderTargetTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pRenderTargetTexture, &renderTargetViewDesc, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create RenderTargetView failed", 0, 0);
		exit(-1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = renderTargetTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(pRenderTargetTexture, &shaderResourceViewDesc, &pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create CreateShaderResourceView failed", 0, 0);
		exit(-1);
	}

	///////////////////////////////////////////

}


RenderTarget::~RenderTarget()
{

	SAFE_RELEASE(pRasterizerStateCullBack);
	SAFE_RELEASE(pRasterizerStateCullFront);
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pVSProjectionCBuffer);
	
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pRenderTargetTexture);
	SAFE_RELEASE(pRenderTargetView);


	SAFE_RELEASE(pShaderResourceView);
	releasePixelShader(pPixelShader);
	releaseVertexShader(pVertexShader, pInputLayout);


}

void RenderTarget::render(ID3D11DeviceContext* pDeviceContext)
{

	UINT pStrides = sizeof(vertex);
	UINT pOffsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &pStrides, &pOffsets);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*//*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);
	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
	pDeviceContext->Draw(vertexCount, 0);

	float ClearColor[4] = { 128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 0.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	setRenderTargetWH(virtualWidth, virtualHeight);

	e_mainCamera.viewPort.Width = virtualWidth;
	e_mainCamera.viewPort.Height = virtualHeight;
	pDeviceContext->RSSetViewports(1, &e_mainCamera.viewPort);
}

void RenderTarget::render(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[])
{

	D3D11_MAPPED_SUBRESOURCE mappedSubRec;
	hr = pDeviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD/*D3D11_MAP_WRITE_NO_OVERWRITE*/, 0, &mappedSubRec);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Render Map failed", 0, 0);
		return;
	}
	memcpy(mappedSubRec.pData, pCoordNDC, sizeof(vertex)*vertexCount);
	pDeviceContext->Unmap(pVertexBuffer, 0);

	render(pDeviceContext);

}

void RenderTarget::render(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, bool doReflection)
{
	if ((int)srcWidth == 0 || (int)srcHeight == 0)
	{
		srcWidth = drawWidth;
		srcHeight = drawHeight;
	}

	// Caculate 2D texture coordinate and update vertics buffer
	XMFLOAT4 colorNDC;
	colorNDC = toNDColor(blendColor);
	vertex vertices[] = {
		{
			XMFLOAT3(drawX, drawY, 0),								XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(srcX, srcY)
		},
		{
			XMFLOAT3(drawX + drawWidth, drawY, 0),				XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(srcX + srcWidth, srcY)
		},
		{
			XMFLOAT3(drawX, drawY + drawHeight, 0),				XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(srcX , srcY + srcHeight)
		},
		{
			XMFLOAT3(drawX + drawWidth, drawY + drawHeight, 0),	XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w),
			XMFLOAT2(srcX + srcWidth, srcY + srcHeight)
		},
	};



	XMFLOAT3 center(drawX + drawWidth / 2, drawY + drawHeight / 2, 0);

	// Rotation And Change to NDC coordinate
	float angleRadian = rotateAngle * 0.01745/*(M_PI / 180,0f)*/;
	for (unsigned int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = rotationZ(vertices[i].position, angleRadian, center);
		vertices[i].position = toNDC(vertices[i].position);
		vertices[i].texcoord = toNDC_UV(vertices[i].texcoord);
		vertices[i].normal = { 0,0,-1 };
		if (doReflection)
		{
			vertices[i].normal.x = -vertices[i].normal.x;
			vertices[i].normal.y = -vertices[i].normal.y;
			vertices[i].normal.z = -vertices[i].normal.z;
		}
	}
	if (doReflection)
	{
		static XMFLOAT2 swapTemp;
		swapTemp = vertices[0].texcoord;
		vertices[0].texcoord = vertices[1].texcoord;
		vertices[1].texcoord = swapTemp;
		swapTemp = vertices[2].texcoord;
		vertices[2].texcoord = vertices[3].texcoord;
		vertices[3].texcoord = swapTemp;
		pDeviceContext->RSSetState(pRasterizerStateCullFront);
	}
	else
	{
		pDeviceContext->RSSetState(pRasterizerStateCullBack);
	}

	render(pDeviceContext, vertices);
}

void RenderTarget::setProjection(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const Transform& transform)
{
	static XMFLOAT3 positionNDC, rotationAxisNDC;
	positionNDC = toNDC(transform.position);
	rotationAxisNDC = toNDC(transform.rotationAxis);
	static XMMATRIX S, R, T, W, V, P, WVP;
	S = R = T = W = V = P = WVP = DirectX::XMMatrixIdentity();
	//S = DirectX::XMMatrixScaling(transform.scaling.x, transform.scaling.y, transform.scaling.z);
	////R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxisNDC.x, rotationAxisNDC.y, rotationAxisNDC.z, 0), transform.rotateAngle*0.01745329251);
	//R = DirectX::XMMatrixRotationRollPitchYaw(transform.eulerAngle.y*0.01745, transform.eulerAngle.x*0.01745, transform.eulerAngle.z*0.01745);
	//T = DirectX::XMMatrixTranslation(positionNDC.x, positionNDC.y, positionNDC.z);
	//W = S*R*T;
	//R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxisNDC.x, rotationAxisNDC.y, rotationAxisNDC.z, 0), transform2D3D->angle*0.01745329251);
	R = DirectX::XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), transform.eulerAngle.x*0.01745)
		*DirectX::XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), transform.eulerAngle.y*0.01745)
		*DirectX::XMMatrixRotationAxis(XMVectorSet(0, 0, 1, 0), transform.eulerAngle.z*0.01745);
	//R = DX::XMMatrixRotationRollPitchYaw(transform.eulerAngle.y*0.01745, transform.eulerAngle.x*0.01745, transform.eulerAngle.z*0.01745);
	S = DirectX::XMMatrixScaling(transform.scaling.x, transform.scaling.y, transform.scaling.z);
	T = DirectX::XMMatrixTranslation(positionNDC.x, positionNDC.y, positionNDC.z);
	W = T*R*S;

	//e_mainCamera.clear();
	V = DirectX::XMMatrixLookAtLH(e_mainCamera.eyePosition, e_mainCamera.focusPosition, e_mainCamera.upDirection);
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
	lightV = (e_mainCamera.focusPosition - e_mainCamera.eyePosition);
	updateCbuffer.lightDirection = XMFLOAT4(lightV.vector4_f32[0], lightV.vector4_f32[1], lightV.vector4_f32[2], 0.0f);
	updateCbuffer.materialColor = XMFLOAT4(0.8, 0.8, 0, 1);

	pDeviceContext->UpdateSubresource(pVSProjectionCBuffer, 0, NULL, &updateCbuffer, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pVSProjectionCBuffer);
}

void RenderTarget::render3D(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, float rotateAngle, UINTCOLOR blendColor, const Transform& transform, bool doReflection)
{
	// 
	virtualWidth = srcWidth;
	virtualHeight = srcHeight;
	setProjection(pDeviceContext, XMFLOAT3(0, 0, 0), transform);
	render(pDeviceContext, drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, rotateAngle, blendColor, doReflection);
}

XMFLOAT3 RenderTarget::toNDC(float screenX, float screenY)
{
	float x, y;
	x = 2.0f*screenX / SCREEN_WIDTH - 1.0f;
	y = 1.0f - 2.0f*screenY / SCREEN_HEIGHT;
	return XMFLOAT3(x, y, 0);
}

XMFLOAT3 RenderTarget::toNDC(XMFLOAT3 coord)
{
	float x, y, z;
	// 2D without projection
	/*x = 2.0f*_coord.x / SCREEN_WIDTH - 1.0f;
	y = 1.0f - 2.0f*_coord.y / SCREEN_WIDTH;*/

	// 2D with projection
	x = coord.x / (float)SCREEN_WIDTH;
	y = -coord.y / (float)SCREEN_WIDTH;
	z = coord.z / (float)SCREEN_WIDTH;
	return XMFLOAT3(x, y, z);
}

// Screen coordinate to UV NDC coordinate
XMFLOAT2 RenderTarget::toNDC_UV(XMFLOAT2 coord)
{
	float x, y;
	float imgWidth, imgHeight;
	imgWidth = renderTargetTextureDesc.Width;
	imgHeight = renderTargetTextureDesc.Height;

	x = coord.x / imgWidth;
	y = coord.y / imgHeight;
	return XMFLOAT2(x, y);
}

XMFLOAT3 RenderTarget::rotationZ(XMFLOAT3 coord, float rotateAngle, XMFLOAT3 centerCoord)
{
	XMFLOAT3 P, Pr;
	P.x = coord.x - centerCoord.x;
	P.y = coord.y - centerCoord.y;
	P.z = 0/*_coord.z - _center.z*/;
	// rotation
	Pr.x = P.x*cosf(rotateAngle) - P.y*sinf(rotateAngle) + centerCoord.x;
	Pr.y = P.y*cosf(rotateAngle) + P.x*sinf(rotateAngle) + centerCoord.y;
	Pr.z = 0;
	return Pr;
}


