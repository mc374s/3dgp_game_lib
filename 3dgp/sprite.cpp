#include <iostream>

#include "sprite.h"
//#define _CRT_SECURE_NO_WARNINGS

bool Sprite::initialize(ID3D11Device* pDevice)
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

	// create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Initialize hr failed", 0, 0);
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
		MessageBox(0, L"Create pConstantBuffer failed", L"Sprite::Initialize()", 0);
		return false;
	}

	return true;
}

Sprite::Sprite(ID3D11Device* pDevice)
{
	initialize(pDevice);

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	RM::loadVertexShader(pDevice, "Data/Shader/texture_off_2d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);
	RM::loadPixelShader(pDevice, "Data/Shader/texture_off_ps.cso", &pPixelShader);

	pShaderResourceView = NULL;

}


Sprite::Sprite(ID3D11Device* pDevice, char* pFilename/*Texture file name*/, bool doProjection)
{
	initialize(pDevice);
	doProjection = doProjection;


	// Define the input layout
	if (doProjection) {
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		RM::loadVertexShader(pDevice, "Data/Shader/texture_on_3d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);
	}
	else {
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		RM::loadVertexShader(pDevice, "Data/Shader/texture_on_2d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);
	}
	RM::loadPixelShader(pDevice, "Data/Shader/texture_on_ps.cso", &pPixelShader);

	ID3D11Resource* resource = NULL;
	RM::loadShaderResourceView(pDevice, pFilename, &resource, &pShaderResourceView);


	// TEXTURE2D_DESC Initialize
	renderTargetTextureDesc.MipLevels = 1;
	renderTargetTextureDesc.ArraySize = 1;
	renderTargetTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetTextureDesc.SampleDesc.Count = 1;
	renderTargetTextureDesc.SampleDesc.Quality = 0;
	renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTargetTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	renderTargetTextureDesc.CPUAccessFlags = 0;
	renderTargetTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture2d;
	if (resource == NULL)
	{
		MessageBox(0, L"sprite: Create Recource failed", 0, 0);
		return;
	}
	resource->QueryInterface(&texture2d);
	texture2d->GetDesc(&renderTargetTextureDesc);

	// SAMPLER_DESC Initialize

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	/*samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;*/
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create SamplerState failed", 0, 0);
		return;
	}

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	if (doProjection) {
		depthStencilDesc.DepthEnable = TRUE;
	}
	else {
		depthStencilDesc.DepthEnable = FALSE;
	}
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create DepthStencilState failed", L"Sprite::Initialize", 0);
		return;
	}


	if (resource) resource->Release();
	if (texture2d) texture2d->Release();

}

Sprite::~Sprite()
{
	SAFE_RELEASE(pRasterizerState);
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pVSProjectionCBuffer);

	RM::releasePixelShader(pPixelShader);
	RM::releaseVertexShader(pVertexShader, pInputLayout);
	RM::releaseShaderResourceView(pShaderResourceView);
}

void Sprite::render(ID3D11DeviceContext* pDeviceContext)
{

	UINT pStrides = sizeof(vertex);
	UINT pOffsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &pStrides, &pOffsets);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*//*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	//pDeviceContext->PSSetShaderResources(0, 1, getShaderResourceView(srvNO));

	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	pDeviceContext->RSSetState(pRasterizerState);
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
	pDeviceContext->Draw(vertexCount, 0);

}

void Sprite::render(ID3D11DeviceContext* pDeviceContext, vertex pCoordNDC[])
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

void Sprite::render(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float rotateAngle, UINTCOLOR blendColor)
{
	XMFLOAT4 colorNDC;
	colorNDC = toNDColor(blendColor);
	vertex vertices[] = {
		{ XMFLOAT3(drawX, drawY, 0),								XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(0,0) },
		{ XMFLOAT3(drawX + drawWidth, drawY, 0),					XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(1,0) },
		{ XMFLOAT3(drawX, drawY + drawHeight, 0),					XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(0,1) },
		{ XMFLOAT3(drawX + drawWidth, drawY + drawHeight, 0),	XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(1,1) },
	};
	XMFLOAT3 center(drawX + drawWidth / 2, drawY + drawHeight / 2, 0);

	float angleRadian = rotateAngle * 0.01745/*(M_PI / 180,0f)*/;
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = rotationZ(vertices[i].position, angleRadian, center);
		vertices[i].position = toNDC_RT(vertices[i].position);
	}

	render(pDeviceContext, vertices);
}

void Sprite::render(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, UINTCOLOR blendColor, float rotateAngle, bool doCenterRotation, float rotatePosX, float rotatePosY, bool doReflection, int scaleMode)
{
	if ((int)srcWidth == 0 || (int)srcHeight == 0)
	{
		srcWidth = drawWidth;
		srcHeight = drawHeight;
	}

	switch (scaleMode)
	{
	case CENTER:
		drawX += (srcWidth - drawWidth) / 2.0f;
		drawY += (srcHeight - drawHeight) / 2.0f;
		break;
	case LEFTTOP:
		break;
	case TOPCENTER:
		drawX += (srcWidth - drawWidth) / 2.0f;
		break;
	case RIGHTTOP:
		drawX += (srcWidth - drawWidth);
		break;
	case RIGHTCENTER:
		drawX += (srcWidth - drawWidth);
		drawY += (srcHeight - drawHeight) / 2.0f;
		break;
	case RIGHTBOTTOM:
		drawX += (srcWidth - drawWidth);
		drawY += (srcHeight - drawHeight);
		break;
	case BOTTOMCENTER:
		drawX += (srcWidth - drawWidth) / 2.0f;
		drawY += (srcHeight - drawHeight);
		break;
	case LEFTBOTTOM:
		drawY += (srcHeight - drawHeight);
		break;
	case LEFTCENTER:
		drawY += (srcHeight - drawHeight) / 2.0f;
		break;
	default:
		break;
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
	if (doReflection)
	{
		XMFLOAT2 texcoordTemp;
		texcoordTemp = vertices[0].texcoord;
		vertices[0].texcoord = vertices[1].texcoord;
		vertices[1].texcoord = texcoordTemp;
		texcoordTemp = vertices[2].texcoord;
		vertices[2].texcoord = vertices[3].texcoord;
		vertices[3].texcoord = texcoordTemp;
	}

	XMFLOAT3 center;
	center.x = doCenterRotation ? (drawX + drawWidth / 2) : rotatePosX;
	center.y = doCenterRotation ? (drawY + drawHeight / 2) : rotatePosY;
	center.z = 0;
	// Rotation And Change to NDC coordinate
	float angleRadian = rotateAngle * 0.01745/*(M_PI / 180,0f)*/;
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = rotationZ(vertices[i].position, angleRadian, center);
		vertices[i].position = toNDC_RT(vertices[i].position, doProjection);
		vertices[i].texcoord = toNDC_UV(vertices[i].texcoord);
		vertices[i].normal.x = 0.0f;
		vertices[i].normal.y = 0.0f;
		vertices[i].normal.z = -1.0f;
	}

	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);
	pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);

	render(pDeviceContext, vertices);
}

void Sprite::setProjection(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const Transform& transform)
{
	static XMFLOAT3 positionNDC, rotationAxisNDC;
	positionNDC = toNDC_RT(transform.position, doProjection);
	rotationAxisNDC = toNDC_RT(transform.rotationAxis, doProjection);
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

	V = DirectX::XMMatrixLookAtLH(e_mainCamera.eyePosition, e_mainCamera.focusPosition, e_mainCamera.upDirection);
	P = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
	WVP = W*V*P;
	/*W = DirectX::XMMatrixTranspose(W);
	V = DirectX::XMMatrixTranspose(V);
	P = DirectX::XMMatrixTranspose(P);*/

	PROJECTION_CBUFFER updateCbuffer;
	updateCbuffer.world = W;
	updateCbuffer.view = V;
	updateCbuffer.projection = P;
	updateCbuffer.worldViewProjection = WVP;
	//updateCbuffer.lightDirection = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	static XMVECTOR lightDirection = { 0.0f,0.0f,1.0f,0.0f };
	//lightDirection = e_mainCamera.focusPosition - e_mainCamera.eyePosition;
	updateCbuffer.lightDirection = XMFLOAT4(lightDirection.vector4_f32);
	updateCbuffer.materialColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	pDeviceContext->UpdateSubresource(pVSProjectionCBuffer, 0, NULL, &updateCbuffer, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pVSProjectionCBuffer);
}

void Sprite::render3D(ID3D11DeviceContext* pDeviceContext, float drawX, float drawY, float drawWidth, float drawHeight, float srcX, float srcY, float srcWidth, float srcHeight, UINTCOLOR blendColor, float rotateAngle, bool doCenterRotation, float rotatePosX, float rotatePosY, bool doReflection, int scaleMode, const Transform& transform)
{
	setProjection(pDeviceContext, XMFLOAT3(0, 0, 0), transform);
	render(pDeviceContext, drawX, drawY, drawWidth, drawHeight, srcX, srcY, srcWidth, srcHeight, blendColor, rotateAngle, doCenterRotation, rotatePosX, rotatePosY, doReflection, scaleMode);
}


// Screen coordinate to UV NDC coordinate
XMFLOAT2 Sprite::toNDC_UV(XMFLOAT2 coord)
{
	float x, y;
	float imgWidth, imgHeight;
	imgWidth = renderTargetTextureDesc.Width;
	imgHeight = renderTargetTextureDesc.Height;

	x = coord.x / imgWidth;
	y = coord.y / imgHeight;
	return XMFLOAT2(x, y);
}

XMFLOAT3 Sprite::rotationZ(XMFLOAT3 coord, float rotateAngle, XMFLOAT3 centerCoord)
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
