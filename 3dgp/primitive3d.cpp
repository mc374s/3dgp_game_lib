#include "primitive3d.h"

#include "resources_manager.h"

using namespace RM;

Primitive3D::Primitive3D(ID3D11Device *pDevice)
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	loadVertexShader(pDevice, "Data/Shader/texture_off_3d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);

	loadPixelShader(pDevice, "Data/Shader/texture_off_ps.cso", &pPixelShader);

	// create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK/*D3D11_CULL_NONE*//*D3D11_CULL_FRONT*/;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pFillRasterizerState);
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pWireRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create RasterizerState failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//depthStencilDesc.StencilEnable = TRUE;
	//depthStencilDesc.StencilReadMask = 0xFF;
	//depthStencilDesc.StencilWriteMask = 0xFF;
	//// Stencil operations if pixel is front-facing
	//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create DepthStencilState failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	//Initialize(_Device, GEOMETRY_CYLINDER,2,6);

}

void Primitive3D::initialize(ID3D11Device *pDevice, const int &type, const int &_latitudeNum, const int &_longitudeNum)
{
	if (type == GEOMETRY_CUBE)
	{

		// vertex data
		vertex3D vertices[] =
		{
			//UP 
			{ XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3( 1.0f, 1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3( 1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			//UNDER
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			//LEFT
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			//RIGHT
			{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			//FRONT
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			//BACK
			{ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		};

		// index data
		WORD indices[] =
		{
			0,1,2,
			2,1,3,

			4,5,6,
			6,5,7,

			8,9,10,
			10,9,11,

			12,13,14,
			14,13,15,

			16,17,18,
			18,17,19,

			20,21,22,
			21,23,22
		};
		createBuffers(pDevice, vertices, indices);
	}

	if (type == GEOMETRY_CYLINDER)
	{
		latitudeNum = _latitudeNum;
		longitudeNum = _longitudeNum;

		vertexNum = longitudeNum*latitudeNum * 3 + longitudeNum * 2;
		indexNum = longitudeNum*latitudeNum * 2 * 3;

		// Preset Value
		int vertexCount = 0, indexCount = 0;
		//int *latitudeOffSet = new int[latitudeNum + 2];
		float wHalf = 0.2f, hHalf = 0.2f, dHalf = 0.2f;
		float angleHalf = XM_PI / (float)longitudeNum;
		float angle = angleHalf * 2.0f;

		float scaleAdjust = 0.5f;
		float angleY = atanf((1.0f - scaleAdjust)*dHalf / (hHalf*2.0f));

		vertex3D *vertices = new vertex3D[vertexNum];
		WORD *indices = new WORD[indexNum];
		/*vertex3D vertices[48];
		ZeroMemory(vertices, sizeof(vertices));
		WORD indices[72];
		ZeroMemory(indices, sizeof(indices));*/
		int latitudeNumFour = latitudeNum * 4;
		for (int i = 0; i < longitudeNum; i++)
		{
			vertexCount = 0;
			vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(0.0f, hHalf, 0.0f);
			vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// Top
			indices[indexCount] = latitudeNumFour * i + vertexCount;
			indexCount++;
			vertexCount++;
			// TopRight
			indices[indexCount] = latitudeNumFour * ((i + 1) % longitudeNum) + vertexCount;
			indexCount++;
			vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle)*scaleAdjust, hHalf, dHalf*sinf(i*angle)*scaleAdjust);
			vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// TopLeft
			indices[indexCount] = latitudeNumFour * i + vertexCount;
			indexCount++;
			vertexCount++;
			for (int j = 0; j < latitudeNum - 1; j++)
			{
				vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle)*scaleAdjust, hHalf, dHalf*sinf(i*angle)*scaleAdjust);
				vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// LeftTop
				indices[indexCount] = latitudeNumFour * i + vertexCount;
				indexCount++;
				vertexCount++;
				vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle + angle)*scaleAdjust, hHalf, dHalf*sinf(i*angle + angle)*scaleAdjust);
				vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// RightTop
				indices[indexCount] = latitudeNumFour * i + vertexCount;
				indexCount++;
				vertexCount++;
				// LeftBottom
				indices[indexCount] = latitudeNumFour * i + vertexCount;
				indexCount++;

				vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle), -hHalf, dHalf*sinf(i*angle));
				vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// LeftBottom
				indices[indexCount] = latitudeNumFour * i + vertexCount;
				indexCount++;
				// RightTop
				indices[indexCount] = latitudeNumFour * i + vertexCount - 1;
				indexCount++;
				vertexCount++;
				vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle + angle), -hHalf, dHalf*sinf(i*angle + angle));
				vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// RightBottom
				indices[indexCount] = latitudeNumFour * i + vertexCount;
				indexCount++;
				vertexCount++;
			}
			vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle), -hHalf, dHalf*sinf(i*angle));
			vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// BottomRight
			indices[indexCount] = latitudeNumFour * i + vertexCount;
			indexCount++;
			// BottomLeft
			indices[indexCount] = latitudeNumFour * ((i + 1) % longitudeNum) + vertexCount;
			indexCount++;
			vertexCount++;
			vertices[latitudeNumFour * i + vertexCount].position = XMFLOAT3(0.0f, -hHalf, 0.0f);
			vertices[latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// Bottom
			indices[indexCount] = latitudeNumFour * i + vertexCount;
			indexCount++;
			vertexCount++;
		}

		createBuffers(pDevice, vertices, vertexCount, indices, indexCount);
		
		delete vertices;
		delete indices;
	
	}

}



void Primitive3D::createBuffers(ID3D11Device *pDevice, vertex3D *pVertices, int _vertexNum, WORD *pIndices, int _indexNum)
{

	vertexNum = _vertexNum;
	indexNum = _indexNum;

	//pVertices = new vertex3D[_vertexNum];
	//memcpy(pVertices, _vertices, sizeof(vertex3D)*_vertexNum);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT/*D3D11_USAGE_DYNAMIC*/;
	bufferDesc.ByteWidth = sizeof(vertex3D)*(vertexNum + 1);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_WRITE*/0;

	// Create vertex buffer
	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = pVertices;
	hr = pDevice->CreateBuffer(&bufferDesc, &subResourceData, &pVertexBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, L"Create pVertexBuffer failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	//pIndices = new WORD[_indexNum];
	//memcpy(pIndices, _indices, sizeof(WORD)*_indexNum);
	// Create index buffer
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(WORD)*(indexNum);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bufferDesc.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = pIndices;
	hr = pDevice->CreateBuffer(&bufferDesc, &subResourceData, &pIndexBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pIndexBuffer failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	// Create constant buffer
	PROJECTION_CBUFFER VSConstantData;
	//ZeroMemory(&VSConstantData, sizeof(VS_CBUFFER_PROJECTION));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PROJECTION_CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pConstantBuffer failed", L"primitive3D::Primitive3D()", 0);
		return;
	}
}

void Primitive3D::render(ID3D11DeviceContext *pDeviceContext, bool doFill)
{
	if (doFill)
	{
		pDeviceContext->RSSetState(pFillRasterizerState);
	}
	else
	{
		pDeviceContext->RSSetState(pWireRasterizerState);
	}

	UINT pStrides = sizeof(vertex3D);
	UINT pOffsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &pStrides, &pOffsets);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST/*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
	
	pDeviceContext->DrawIndexed(indexNum, 0, 0);
}

void Primitive3D::setProjection(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const XMFLOAT4 &materialColor, const Transform& transform)
{
	static XMFLOAT3 positionNDC, rotationAxisNDC;
	positionNDC = toNDC(transform.position);
	rotationAxisNDC = toNDC(transform.rotationAxis);
	static XMMATRIX S, R, T, W, V, P, WVP;
	S = R = T = W = V = P = WVP = DirectX::XMMatrixIdentity();
	//R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxisNDC.x, rotationAxisNDC.y, rotationAxisNDC.z, 0), transform2D3D->angle*0.01745329251);
	R = DirectX::XMMatrixRotationRollPitchYaw(transform.eulerAngle.y*0.01745, transform.eulerAngle.x*0.01745, transform.eulerAngle.z*0.01745);
	//DirectX::XMQuaternionRotationAxis();
	//DirectX::XMMatrixRotationQuaternion();
	S = DirectX::XMMatrixScaling(transform.scaling.x, transform.scaling.y, transform.scaling.z);
	T = DirectX::XMMatrixTranslation(positionNDC.x, positionNDC.y, positionNDC.z);
	W = S*R*T;
	
	V = DirectX::XMMatrixLookAtLH(e_mainCamera.eyePosition, e_mainCamera.focusPosition, e_mainCamera.upDirection);
	P = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
	/*W = DirectX::XMMatrixTranspose(W);
	V = DirectX::XMMatrixTranspose(V);
	P = DirectX::XMMatrixTranspose(P);*/
	WVP = W*V*P;

	//D3D11_MAPPED_SUBRESOURCE mappedSubRec;
	//hr = _DeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD/*D3D11_MAP_WRITE_NO_OVERWRITE*/, 0, &mappedSubRec);
	//if (FAILED(hr))
	//{
	//	MessageBox(0, L"Update pConstantBuffer failed", L"primitive3D::render()", 0);
	//	return;
	//}

	PROJECTION_CBUFFER updateCbuffer;
	updateCbuffer.world = W;
	updateCbuffer.view = V;
	updateCbuffer.projection = P;
	updateCbuffer.worldViewProjection = WVP; 
	static XMVECTOR lightDirection = { 0.0f,0.0f,1.0f,0.0f };
	lightDirection = e_mainCamera.focusPosition - e_mainCamera.eyePosition;
	updateCbuffer.lightDirection = XMFLOAT4(lightDirection.vector4_f32);
	updateCbuffer.materialColor = materialColor;

	pDeviceContext->UpdateSubresource(pConstantBuffer, 0, NULL, &updateCbuffer, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	//_DeviceContext->Unmap(pConstantBuffer, 0); 
}

void Primitive3D::drawCube(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const XMFLOAT3 &size, const UINTCOLOR &blendColor, const Transform& transform)
{
	//return;
	float x = position.x, y = position.y, z = position.z;
	float w = size.x, h = size.y, d = size.z;
	float wHalf = w / 2.0f, hHalf = h / 2.0f, dHalf = d / 2.0f;
	XMFLOAT4 ndColor = toNDColor(blendColor);
	vertex3D vertices[] =
	{
		//UP
		{ XMFLOAT3(x - wHalf, y + hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y + hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(x - wHalf, y + hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y + hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		//UNDER
		{ XMFLOAT3(x - wHalf, y - hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y - hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(x - wHalf, y - hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y - hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		//LEFT
		{ XMFLOAT3(x - wHalf, y + hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(x - wHalf, y + hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(x - wHalf, y - hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(x - wHalf, y - hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		//RIGHT
		{ XMFLOAT3(x + wHalf, y + hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y + hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y - hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(x + wHalf, y - hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		//FRONT
		{ XMFLOAT3(x - wHalf, y + hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(x + wHalf, y + hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(x - wHalf, y - hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(x + wHalf, y - hHalf, z - dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		//BACK
		{ XMFLOAT3(x + wHalf, y + hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(x - wHalf, y + hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(x + wHalf, y - hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(x - wHalf, y - hHalf, z + dHalf), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};

	UINT vertexCount = ARRAYSIZE(vertices);
	/*for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = toNDC(vertices[i].position);
	}*/
	pDeviceContext->UpdateSubresource(pVertexBuffer, 0, NULL, vertices, 0, 0);

	setProjection(pDeviceContext, position, ndColor, transform);
	render(pDeviceContext, true);
}

void Primitive3D::drawCylinder(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const XMFLOAT3 &size,const Transform& transform)
{

	setProjection(pDeviceContext, position, XMFLOAT4(0.5, 0.5, 0.4, 1.0f), transform);
	render(pDeviceContext, true);
	
}

XMFLOAT3 Primitive3D::toNDC(const XMFLOAT3 &inputCoord)
{
	float x, y, z;
	x = inputCoord.x / (float)SCREEN_WIDTH;
	y = inputCoord.y / (float)SCREEN_WIDTH;
	z = inputCoord.z / (float)SCREEN_WIDTH;
	return XMFLOAT3(x, y, z);
}

Primitive3D::~Primitive3D()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pConstantBuffer);
	SAFE_RELEASE(pIndexBuffer);

	SAFE_RELEASE(pWireRasterizerState);
	SAFE_RELEASE(pFillRasterizerState);

	SAFE_RELEASE(pDepthStencilState);

	SAFE_DELETE(pVertices);
	SAFE_DELETE(pIndices);

	releasePixelShader(pPixelShader);
	releaseVertexShader(pVertexShader, pInputLayout);
}


