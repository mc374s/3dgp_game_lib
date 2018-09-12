#include "primitive3d.h"

#include "resources_manager.h"

using namespace RM;

Primitive3D::Primitive3D(ID3D11Device *a_pDevice)
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	loadVertexShader(a_pDevice, "Data/Shader/texture_off_3d_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &m_pVertexShader, &m_pInputLayout);

	loadPixelShader(a_pDevice, "Data/Shader/texture_off_ps.cso", &m_pPixelShader);

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
	hr = a_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pFillRasterizerState);
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = a_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pWireRasterizerState);
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
	

	hr = a_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create DepthStencilState failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	//Initialize(_Device, GEOMETRY_CYLINDER,2,6);

}

void Primitive3D::initialize(ID3D11Device *a_pDevice, const int &a_type, const int &a_latitudeNum, const int &a_longitudeNum)
{
	if (a_type == GEOMETRY_CUBE)
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
		createBuffers(a_pDevice, vertices, indices);
	}

	if (a_type == GEOMETRY_CYLINDER)
	{
		m_latitudeNum = a_latitudeNum;
		m_longitudeNum = a_longitudeNum;

		m_vertexCount = a_longitudeNum*a_latitudeNum * 3 + a_longitudeNum * 2;
		m_indexCount = a_longitudeNum*a_latitudeNum * 2 * 3;

		// Preset Value
		int vertexCount = 0, indexCount = 0;
		//int *latitudeOffSet = new int[m_latitudeNum + 2];
		float wHalf = 0.2f, hHalf = 0.2f, dHalf = 0.2f;
		float angleHalf = XM_PI / (float)m_longitudeNum;
		float angle = angleHalf * 2.0f;

		float scaleAdjust = 0.5f;
		float angleY = atanf((1.0f - scaleAdjust)*dHalf / (hHalf*2.0f));

		vertex3D *vertices = new vertex3D[m_vertexCount];
		WORD *indices = new WORD[m_indexCount];
		/*vertex3D vertices[48];
		ZeroMemory(vertices, sizeof(vertices));
		WORD indices[72];
		ZeroMemory(indices, sizeof(indices));*/
		int m_latitudeNumFour = m_latitudeNum * 4;
		for (int i = 0; i < m_longitudeNum; i++)
		{
			vertexCount = 0;
			vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(0.0f, hHalf, 0.0f);
			vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// Top
			indices[indexCount] = m_latitudeNumFour * i + vertexCount;
			indexCount++;
			vertexCount++;
			// TopRight
			indices[indexCount] = m_latitudeNumFour * ((i + 1) % m_longitudeNum) + vertexCount;
			indexCount++;
			vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle)*scaleAdjust, hHalf, dHalf*sinf(i*angle)*scaleAdjust);
			vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// TopLeft
			indices[indexCount] = m_latitudeNumFour * i + vertexCount;
			indexCount++;
			vertexCount++;
			for (int j = 0; j < m_latitudeNum - 1; j++)
			{
				vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle)*scaleAdjust, hHalf, dHalf*sinf(i*angle)*scaleAdjust);
				vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// LeftTop
				indices[indexCount] = m_latitudeNumFour * i + vertexCount;
				indexCount++;
				vertexCount++;
				vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle + angle)*scaleAdjust, hHalf, dHalf*sinf(i*angle + angle)*scaleAdjust);
				vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// RightTop
				indices[indexCount] = m_latitudeNumFour * i + vertexCount;
				indexCount++;
				vertexCount++;
				// LeftBottom
				indices[indexCount] = m_latitudeNumFour * i + vertexCount;
				indexCount++;

				vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle), -hHalf, dHalf*sinf(i*angle));
				vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// LeftBottom
				indices[indexCount] = m_latitudeNumFour * i + vertexCount;
				indexCount++;
				// RightTop
				indices[indexCount] = m_latitudeNumFour * i + vertexCount - 1;
				indexCount++;
				vertexCount++;
				vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle + angle), -hHalf, dHalf*sinf(i*angle + angle));
				vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(cosf(i*angle + angleHalf), tan(angleY), sinf(i*angle + angleHalf));
				vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
				// RightBottom
				indices[indexCount] = m_latitudeNumFour * i + vertexCount;
				indexCount++;
				vertexCount++;
			}
			vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(dHalf*cosf(i*angle), -hHalf, dHalf*sinf(i*angle));
			vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// BottomRight
			indices[indexCount] = m_latitudeNumFour * i + vertexCount;
			indexCount++;
			// BottomLeft
			indices[indexCount] = m_latitudeNumFour * ((i + 1) % m_longitudeNum) + vertexCount;
			indexCount++;
			vertexCount++;
			vertices[m_latitudeNumFour * i + vertexCount].position = XMFLOAT3(0.0f, -hHalf, 0.0f);
			vertices[m_latitudeNumFour * i + vertexCount].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[m_latitudeNumFour * i + vertexCount].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
			// Bottom
			indices[indexCount] = m_latitudeNumFour * i + vertexCount;
			indexCount++;
			vertexCount++;
		}

		createBuffers(a_pDevice, vertices, m_vertexCount, indices, m_indexCount);
		
		delete vertices;
		delete indices;
	
	}

}



void Primitive3D::createBuffers(ID3D11Device *a_pDevice, vertex3D *a_pVertices, int a_vertexNum, WORD *a_pIndices, int a_indexNum)
{

	m_vertexCount = a_vertexNum;
	m_indexCount = a_indexNum;

	//m_pVertices = new vertex3D[_vertexNum];
	//memcpy(m_pVertices, _vertices, sizeof(vertex3D)*_vertexNum);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT/*D3D11_USAGE_DYNAMIC*/;
	bufferDesc.ByteWidth = sizeof(vertex3D)*(m_vertexCount + 1);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_WRITE*/0;

	// Create vertex buffer
	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = a_pVertices;
	hr = a_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, L"Create m_pVertexBuffer failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	//m_pIndices = new WORD[_indexNum];
	//memcpy(m_pIndices, _indices, sizeof(WORD)*_indexNum);
	// Create index buffer
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(WORD)*(m_indexCount);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bufferDesc.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = a_pIndices;
	hr = a_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pIndexBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create m_pIndexBuffer failed", L"primitive3D::Primitive3D()", 0);
		return;
	}

	// Create constant buffer
	PROJECTION_CBUFFER VSConstantData;
	//ZeroMemory(&VSConstantData, sizeof(VS_CBUFFER_PROJECTION));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PROJECTION_CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = a_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create m_pConstantBuffer failed", L"primitive3D::Primitive3D()", 0);
		return;
	}
}

void Primitive3D::render(ID3D11DeviceContext *a_pDeviceContext, bool a_doFill)
{
	if (a_doFill)
	{
		a_pDeviceContext->RSSetState(m_pFillRasterizerState);
	}
	else
	{
		a_pDeviceContext->RSSetState(m_pWireRasterizerState);
	}

	UINT pStrides = sizeof(vertex3D);
	UINT pOffsets = 0;
	a_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &pStrides, &pOffsets);
	a_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	a_pDeviceContext->IASetPrimitiveTopology(/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST/*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	a_pDeviceContext->IASetInputLayout(m_pInputLayout);
	a_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	a_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	a_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	
	a_pDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void Primitive3D::setProjection(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const XMFLOAT4 &a_materialColor, const Transform* a_pCustom3D)
{
	if (a_pCustom3D == nullptr)
	{
		Transform init;
		a_pCustom3D = &init;
	}
	static XMFLOAT3 position, rotationAxis;
	position = toNDC(a_pCustom3D->position);
	rotationAxis = toNDC(a_pCustom3D->rotationAxis);
	static XMMATRIX S, R, T, W, V, P, WVP;
	S = R = T = W = V = P = WVP = DirectX::XMMatrixIdentity();
	//R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0), _custom3D->angle*0.01745329251);
	R = DirectX::XMMatrixRotationRollPitchYaw(a_pCustom3D->eulerAngle.y*0.01745, a_pCustom3D->eulerAngle.x*0.01745, a_pCustom3D->eulerAngle.z*0.01745);
	S = DirectX::XMMatrixScaling(a_pCustom3D->scaling.x, a_pCustom3D->scaling.y, a_pCustom3D->scaling.z);
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	W = S*R*T;
	
	V = DirectX::XMMatrixLookAtLH(e_mainCamera.eyePosition, e_mainCamera.focusPosition, e_mainCamera.upDirection);
	P = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
	/*W = DirectX::XMMatrixTranspose(W);
	V = DirectX::XMMatrixTranspose(V);
	P = DirectX::XMMatrixTranspose(P);*/
	WVP = W*V*P;

	//D3D11_MAPPED_SUBRESOURCE mappedSubRec;
	//hr = _DeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD/*D3D11_MAP_WRITE_NO_OVERWRITE*/, 0, &mappedSubRec);
	//if (FAILED(hr))
	//{
	//	MessageBox(0, L"Update m_pConstantBuffer failed", L"primitive3D::render()", 0);
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
	updateCbuffer.materialColor = a_materialColor;

	a_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &updateCbuffer, 0, 0);
	a_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//_DeviceContext->Unmap(m_pConstantBuffer, 0); 
}

void Primitive3D::drawCube(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const XMFLOAT3 &a_size, const UINTCOLOR &a_blendColor, const Transform* a_pCustom3D)
{
	//return;
	int x = a_position.x, y = a_position.y, z = a_position.z;
	float w = a_size.x, h = a_size.y, d = a_size.z;
	float wHalf = w / 2.0f, hHalf = h / 2.0f, dHalf = d / 2.0f;
	XMFLOAT4 ndColor = toNDColor(a_blendColor);
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
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = toNDC(vertices[i].position);
	}
	a_pDeviceContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, vertices, 0, 0);

	setProjection(a_pDeviceContext, a_position, ndColor, a_pCustom3D);
	render(a_pDeviceContext, true);
}

void Primitive3D::drawCylinder(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const XMFLOAT3 &a_size,const Transform* a_pCustom3D)
{

	setProjection(a_pDeviceContext, a_position, XMFLOAT4(0.5, 0.5, 0.4, 1.0f), a_pCustom3D);
	render(a_pDeviceContext, true);
	
}

XMFLOAT3 Primitive3D::toNDC(const XMFLOAT3 &a_inputCoord)
{
	float x, y, z;
	x = a_inputCoord.x / (float)SCREEN_WIDTH;
	y = a_inputCoord.y / (float)SCREEN_WIDTH;
	z = a_inputCoord.z / (float)SCREEN_WIDTH;
	return XMFLOAT3(x, y, z);
}

Primitive3D::~Primitive3D()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	SAFE_RELEASE(m_pWireRasterizerState);
	SAFE_RELEASE(m_pFillRasterizerState);

	SAFE_RELEASE(m_pDepthStencilState);

	SAFE_DELETE(m_pVertices);
	SAFE_DELETE(m_pIndices);

	releasePixelShader(m_pPixelShader);
	releaseVertexShader(m_pVertexShader, m_pInputLayout);
}


