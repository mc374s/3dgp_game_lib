#include <iostream>

#include "sprite.h"
//#define _CRT_SECURE_NO_WARNINGS

bool Sprite::initialize(ID3D11Device* a_pDevice)
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

	// create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	hr = a_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
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
	hr = a_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pVSProjectionCBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create m_pConstantBuffer failed", L"Sprite::Initialize()", 0);
		return false;
	}

	return true;
}

Sprite::Sprite(ID3D11Device* a_pDevice)
{
	initialize(a_pDevice);

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	RM::loadVertexShader(a_pDevice, "Data/Shader/texture_off_2d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &m_pVertexShader, &m_pInputLayout);
	RM::loadPixelShader(a_pDevice, "Data/Shader/texture_off_ps.cso", &m_pPixelShader);

	m_pShaderResourceView = NULL;

}


Sprite::Sprite(ID3D11Device* a_pDevice, char* a_pFilename/*Texture file name*/, bool a_doProjection)
{
	initialize(a_pDevice);
	m_doProjection = a_doProjection;


	// Define the input layout
	if (m_doProjection) {
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		RM::loadVertexShader(a_pDevice, "Data/Shader/texture_on_3d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &m_pVertexShader, &m_pInputLayout);
	}
	else {
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		RM::loadVertexShader(a_pDevice, "Data/Shader/texture_on_2d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &m_pVertexShader, &m_pInputLayout);
	}
	RM::loadPixelShader(a_pDevice, "Data/Shader/texture_on_ps.cso", &m_pPixelShader);

	ID3D11Resource* resource = NULL;
	RM::loadShaderResourceView(a_pDevice, a_pFilename, &resource, &m_pShaderResourceView);


	// TEXTURE2D_DESC Initialize
	m_renderTargetTextureDesc.MipLevels = 1;
	m_renderTargetTextureDesc.ArraySize = 1;
	m_renderTargetTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_renderTargetTextureDesc.SampleDesc.Count = 1;
	m_renderTargetTextureDesc.SampleDesc.Quality = 0;
	m_renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	m_renderTargetTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_renderTargetTextureDesc.CPUAccessFlags = 0;
	m_renderTargetTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture2d;
	if (resource == NULL)
	{
		MessageBox(0, L"sprite: Create Recource failed", 0, 0);
		return;
	}
	resource->QueryInterface(&texture2d);
	texture2d->GetDesc(&m_renderTargetTextureDesc);

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

	hr = a_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Create SamplerState failed", 0, 0);
		return;
	}

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	if (m_doProjection) {
		depthStencilDesc.DepthEnable = TRUE;
	}
	else {
		depthStencilDesc.DepthEnable = FALSE;
	}
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = a_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
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
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pVSProjectionCBuffer);

	RM::releasePixelShader(m_pPixelShader);
	RM::releaseVertexShader(m_pVertexShader, m_pInputLayout);
	RM::releaseShaderResourceView(m_pShaderResourceView);
}

void Sprite::render(ID3D11DeviceContext* a_pDeviceContext)
{

	UINT pStrides = sizeof(vertex);
	UINT pOffsets = 0;
	a_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &pStrides, &pOffsets);
	a_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*//*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	//a_pDeviceContext->PSSetShaderResources(0, 1, getShaderResourceView(m_srvNO));

	a_pDeviceContext->IASetInputLayout(m_pInputLayout);
	a_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	a_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	a_pDeviceContext->RSSetState(m_pRasterizerState);
	a_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	a_pDeviceContext->Draw(m_vertexCount, 0);

}

void Sprite::render(ID3D11DeviceContext* a_pDeviceContext, vertex a_pCoordNDC[])
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

void Sprite::render(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_rotateAngle, UINTCOLOR a_blendColor)
{
	XMFLOAT4 colorNDC;
	colorNDC = toNDColor(a_blendColor);
	vertex vertices[] = {
		{ XMFLOAT3(a_drawX, a_drawY, 0),								XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(0,0) },
		{ XMFLOAT3(a_drawX + a_drawWidth, a_drawY, 0),					XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(1,0) },
		{ XMFLOAT3(a_drawX, a_drawY + a_drawHeight, 0),					XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(0,1) },
		{ XMFLOAT3(a_drawX + a_drawWidth, a_drawY + a_drawHeight, 0),	XMFLOAT4(colorNDC.x, colorNDC.y, colorNDC.z, colorNDC.w), XMFLOAT2(1,1) },
	};
	XMFLOAT3 center(a_drawX + a_drawWidth / 2, a_drawY + a_drawHeight / 2, 0);

	float angleRadian = a_rotateAngle * 0.01745/*(M_PI / 180,0f)*/;
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = rotationZ(vertices[i].position, angleRadian, center);
		vertices[i].position = toNDC_RT(vertices[i].position);
	}

	render(a_pDeviceContext, vertices);
}

void Sprite::render(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX, float a_srcY, float a_srcWidth, float a_srcHeight, UINTCOLOR a_blendColor, float a_rotateAngle, bool a_doCenterRotation, float a_rotatePosX, float a_rotatePosY, bool a_doReflection, int a_scaleMode)
{
	if ((int)a_srcWidth == 0 || (int)a_srcHeight == 0)
	{
		a_srcWidth = a_drawWidth;
		a_srcHeight = a_drawHeight;
	}

	switch (a_scaleMode)
	{
	case CENTER:
		a_drawX += (a_srcWidth - a_drawWidth) / 2.0f;
		a_drawY += (a_srcHeight - a_drawHeight) / 2.0f;
		break;
	case LEFTTOP:
		break;
	case TOPCENTER:
		a_drawX += (a_srcWidth - a_drawWidth) / 2.0f;
		break;
	case RIGHTTOP:
		a_drawX += (a_srcWidth - a_drawWidth);
		break;
	case RIGHTCENTER:
		a_drawX += (a_srcWidth - a_drawWidth);
		a_drawY += (a_srcHeight - a_drawHeight) / 2.0f;
		break;
	case RIGHTBOTTOM:
		a_drawX += (a_srcWidth - a_drawWidth);
		a_drawY += (a_srcHeight - a_drawHeight);
		break;
	case BOTTOMCENTER:
		a_drawX += (a_srcWidth - a_drawWidth) / 2.0f;
		a_drawY += (a_srcHeight - a_drawHeight);
		break;
	case LEFTBOTTOM:
		a_drawY += (a_srcHeight - a_drawHeight);
		break;
	case LEFTCENTER:
		a_drawY += (a_srcHeight - a_drawHeight) / 2.0f;
		break;
	default:
		break;
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
	if (a_doReflection)
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
	center.x = a_doCenterRotation ? (a_drawX + a_drawWidth / 2) : a_rotatePosX;
	center.y = a_doCenterRotation ? (a_drawY + a_drawHeight / 2) : a_rotatePosY;
	center.z = 0;
	// Rotation And Change to NDC coordinate
	float angleRadian = a_rotateAngle * 0.01745/*(M_PI / 180,0f)*/;
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = rotationZ(vertices[i].position, angleRadian, center);
		vertices[i].position = toNDC_RT(vertices[i].position, m_doProjection);
		vertices[i].texcoord = toNDC_UV(vertices[i].texcoord);
		vertices[i].normal.x = 0.0f;
		vertices[i].normal.y = 0.0f;
		vertices[i].normal.z = -1.0f;
	}

	a_pDeviceContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
	a_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	render(a_pDeviceContext, vertices);
}

void Sprite::setProjection(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const CUSTOM3D* a_pCustom3D)
{
	if (a_pCustom3D == nullptr)
	{
		CUSTOM3D init;
		a_pCustom3D = &init;
	}
	static XMFLOAT3 position, rotationAxis;
	position = toNDC_RT(a_pCustom3D->position, m_doProjection);
	rotationAxis = toNDC_RT(a_pCustom3D->rotationAxis, m_doProjection);
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

	a_pDeviceContext->UpdateSubresource(m_pVSProjectionCBuffer, 0, NULL, &updateCbuffer, 0, 0);
	a_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pVSProjectionCBuffer);
}

void Sprite::render3D(ID3D11DeviceContext* a_pDeviceContext, float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX, float a_srcY, float a_srcWidth, float a_srcHeight, UINTCOLOR a_blendColor, float a_rotateAngle, bool a_doCenterRotation, float a_rotatePosX, float a_rotatePosY, bool a_doReflection, int a_scaleMode, const CUSTOM3D* a_pCustom3D)
{
	setProjection(a_pDeviceContext, XMFLOAT3(0, 0, 0), a_pCustom3D);
	render(a_pDeviceContext, a_drawX, a_drawY, a_drawWidth, a_drawHeight, a_srcX, a_srcY, a_srcWidth, a_srcHeight, a_blendColor, a_rotateAngle, a_doCenterRotation, a_rotatePosX, a_rotatePosY, a_doReflection, a_scaleMode);
}


// Screen coordinate to UV NDC coordinate
XMFLOAT2 Sprite::toNDC_UV(XMFLOAT2 a_coord)
{
	float x, y;
	float imgWidth, imgHeight;
	imgWidth = m_renderTargetTextureDesc.Width;
	imgHeight = m_renderTargetTextureDesc.Height;

	x = a_coord.x / imgWidth;
	y = a_coord.y / imgHeight;
	return XMFLOAT2(x, y);
}

XMFLOAT3 Sprite::rotationZ(XMFLOAT3 a_coord, float a_rotateAngle, XMFLOAT3 a_centerCoord)
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
