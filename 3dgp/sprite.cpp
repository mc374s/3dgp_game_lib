#include "sprite.h"
#include "resources_manager.h"
#include "state_subset.h"
#include "texture.h"

#include "VertexTypes.h"

using namespace DirectX;
using namespace GLC;

#define VERTICES_TOTAL (4)

// Initial 4 vertices' value; usage: draw method's vertex transform.
const VertexPositionNormalColorTexture verticesInitValue[VERTICES_TOTAL] = {
	{ XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -1), XMFLOAT4(1, 1, 1, 1), XMFLOAT2(0,0) },
	{ XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, -1), XMFLOAT4(1, 1, 1, 1), XMFLOAT2(1,0) },
	{ XMFLOAT3(0, 1, 0), XMFLOAT3(0, 0, -1), XMFLOAT4(1, 1, 1, 1), XMFLOAT2(0,1) },
	{ XMFLOAT3(1, 1, 0), XMFLOAT3(0, 0, -1), XMFLOAT4(1, 1, 1, 1), XMFLOAT2(1,1) },
};

// Roate @position by @rotation around point @origin.
inline XMFLOAT3 XM_CALLCONV Rotation(FXMVECTOR position, FXMVECTOR rotation, FXMVECTOR origin)
{
	//XMFLOAT3 result(coord.x - centerCoord.x, coord.y - centerCoord.y, 0);
	//// Rotation around Z-Axis
	//result.x = result.x*cosf(rotateAngle) - result.y*sinf(rotateAngle) + centerCoord.x;
	//result.y = result.x*sinf(rotateAngle) + result.y*cosf(rotateAngle) + centerCoord.y;

	XMVECTOR result = position - origin;
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYawFromVector(rotation);

	result = XMVector3Rotate(result, rotationQuaternion) + origin;

	XMFLOAT3 resultXMF3;
	XMStoreFloat3(&resultXMF3, result);

	return resultXMF3;
}

// D3D COM holder
class Sprite::Impl{
private:
	ID3D11SamplerState*			pSamplerState;
	ID3D11RasterizerState*		pRasterizerState;
	ID3D11DepthStencilState*	pDepthStencilState;
	
public:
	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pVSProjectionCBuffer;

	ID3D11ShaderResourceView*	pShaderResourceView;
	ID3D11InputLayout*			pInputLayout;
	ID3D11VertexShader*			pVertexShader;
	ID3D11PixelShader*			pPixelShader;

	ID3D11Device*				pDevice;
	ID3D11DeviceContext*		pDeviceContext;

	VertexPositionNormalColorTexture vertices[VERTICES_TOTAL];

	Impl(ID3D11DeviceContext* pDeviceContext);
	~Impl();

	// Set devices and draw vertices.
	void Draw();

};

// Add Devices and States references, create buffers.
Sprite::Impl::Impl(ID3D11DeviceContext* pDeviceContext)
{
	// Create Devices reference.
	this->pDeviceContext = pDeviceContext;
	pDeviceContext->GetDevice(&pDevice);

	// Create sampler state reference.
	pSamplerState = Sampler::State(Sampler::WRAP);
	// Create rasterizer state reference.
	pRasterizerState = Rasterizer::State::Get(Rasterizer::CULL_NONE);
	// Create depth stencil state reference.
	pDepthStencilState = DepthStencil::State::Get(DepthStencil::NONE);

	// Create buffers
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*//*D3D11_USAGE_DEFAULT*/D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexPositionNormalColorTexture) * VERTICES_TOTAL;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE/*0*/;

	// Create vertex buffer
	HRESULT hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"sprite: Initialize pVertexBuffer failed", 0, 0);
	}

	// Create constant buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(PROJECTION_CBUFFER);
	hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pVSProjectionCBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pConstantBuffer failed", L"Sprite::Initialize()", 0);
	}

	// Initialize vertices 
	memcpy(vertices, verticesInitValue, sizeof(verticesInitValue));

}

Sprite::Impl::~Impl()
{
	// Release D3D resources' reference
	pRasterizerState = nullptr;
	pSamplerState = nullptr;
	pDepthStencilState = nullptr;

	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pVSProjectionCBuffer);

	RM::ReleasePixelShader(pPixelShader);
	RM::ReleaseVertexShader(pVertexShader, pInputLayout);
	RM::ReleaseShaderResourceView(pShaderResourceView);

	// Release Devices' reference
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pDeviceContext);
	
}

// Set Devices and draw vertices.
void Sprite::Impl::Draw()
{
	// Update vertex buffer and set primitive topology.
	D3D11_MAPPED_SUBRESOURCE mappedSubRec;
	if (FAILED(pDeviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubRec))){
		MessageBox(0, L"sprite: Draw Map failed", 0, 0);
		return;
	}
	memcpy(mappedSubRec.pData, vertices, sizeof(VertexPositionNormalColorTexture)*VERTICES_TOTAL);
	pDeviceContext->Unmap(pVertexBuffer, 0);
	UINT pStrides = sizeof(VertexPositionNormalColorTexture);
	UINT pOffsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &pStrides, &pOffsets);

	// Set shaders, states and primitive topology.
	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	pDeviceContext->RSSetState(pRasterizerState);
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*//*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);

	// Texture exist.
	if (pShaderResourceView) {
		pDeviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);
		pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	}

	// Draw vertex buffer that bound in pipleline to back buffer.
	pDeviceContext->Draw(VERTICES_TOTAL, 0);
}


Sprite::Sprite(ID3D11DeviceContext* pDeviceContext) :pImpl(new Impl(pDeviceContext))
{
	// Without texture
	
	// Create the input layout and shaders references.
	/*RM::LoadVertexShader(pImpl->pDevice, "./Data/Shaders/common_vs_diffuse.cso",
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		&pImpl->pVertexShader,
		&pImpl->pInputLayout);
	RM::LoadPixelShader(pImpl->pDevice, "./Data/Shaders/common_ps_diffuse.cso", &pImpl->pPixelShader);*/

	pImpl->pShaderResourceView = NULL;

	// For testing
	// 2D coordinate layout and vertex shader.
	RM::LoadVertexShader(pImpl->pDevice, "./Data/Shaders/common_vs_diffuse_tex.cso",
		VertexPositionNormalColorTexture::InputElements,
		VertexPositionNormalColorTexture::InputElementCount,
		&pImpl->pVertexShader,
		&pImpl->pInputLayout);
	RM::LoadPixelShader(pImpl->pDevice, "./Data/Shaders/common_ps_diffuse_tex.cso", &pImpl->pPixelShader);

}


Sprite::Sprite(ID3D11DeviceContext* pDeviceContext, char* pFilename, bool doProjection) :pImpl(new Impl(pDeviceContext))
{
	// With texture
	
	// Create ShaderResourceView reference.
	RM::LoadShaderResourceView(pImpl->pDevice, &pImpl->pShaderResourceView, pFilename, nullptr);

	// Create the input layout and shaders references.
	char vertexShaderName[256];
	if (doProjection) {
		// 3D coordinate layout and vertex shader.
		strcat_s(vertexShaderName, "./Data/Shaders/common_vs_diffuse_tex_normal.cso");
	}
	else {
		// 2D coordinate layout and vertex shader.
		strcat_s(vertexShaderName, "./Data/Shaders/common_vs_diffuse_tex.cso");
	}

	RM::LoadVertexShader(pImpl->pDevice, vertexShaderName,
		VertexPositionNormalColorTexture::InputElements,
		VertexPositionNormalColorTexture::InputElementCount,
		&pImpl->pVertexShader,
		&pImpl->pInputLayout);
	RM::LoadPixelShader(pImpl->pDevice, "./Data/Shaders/common_ps_diffuse_tex.cso", &pImpl->pPixelShader);
	
}

Sprite::~Sprite()
{
	SAFE_DELETE(pImpl);
}

// Update constant buffer
void XM_CALLCONV Sprite::Draw(Texture* texture,
	FXMVECTOR sourceRectangle,
	FXMMATRIX view,
	CXMMATRIX projection,
	FXMVECTOR position,
	FXMVECTOR scale,
	CXMVECTOR rotation,
	CXMVECTOR origin,
	CXMVECTOR blendColor)
{
	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&updateCbuffer.world, world);
	XMStoreFloat4x4(&updateCbuffer.worldViewProjection, world*view*projection);
	updateCbuffer.lightDirection = { XMVectorGetZ(view.r[0]), XMVectorGetZ(view.r[1]), XMVectorGetZ(view.r[2]), 1 };
	XMStoreFloat4(&updateCbuffer.materialColor, blendColor);
	//lightDirection = Camera::mainCamera.focusPosition - Camera::mainCamera.eyePosition;
	updateCbuffer.materialColor = { 1,1,1,1 };

	pImpl->pDeviceContext->UpdateSubresource(pImpl->pVSProjectionCBuffer, 0, NULL, &updateCbuffer, 0, 0);
	pImpl->pDeviceContext->VSSetConstantBuffers(3, 1, &pImpl->pVSProjectionCBuffer);

	Draw(texture, sourceRectangle, position, scale, rotation, blendColor);
}

// Update vertices
void XM_CALLCONV Sprite::Draw(Texture* texture,
	FXMVECTOR sourceRectangle,
	FXMVECTOR position,
	FXMVECTOR scale,
	CXMVECTOR rotation,
	CXMVECTOR origin,
	CXMVECTOR blendColor)
{
	// If @texture exist, set ShaderResourceView reference
	if (texture) {
		pImpl->pShaderResourceView = texture->GetData();
	}
	else {
		// TODO: if @texture==nullptr, draw a coloring rectangle
		return;
	}

	// Reset vertices.
	memcpy(pImpl->vertices, verticesInitValue, sizeof(verticesInitValue));

	// Get current viewport(screen)'s width and height.
	static D3D11_VIEWPORT viewPort;
	static UINT viewPortNum = 1;
	pImpl->pDeviceContext->RSGetViewports(&viewPortNum, &viewPort);

	// Calculate every vertex's position, texture coordinate and vertex color.
	// Convert all of them to NDC.
	XMFLOAT4 color;
	XMStoreFloat4(&color, blendColor);

	for (int i = 0; i < VERTICES_TOTAL; i++)
	{
		// Calculate vertex position after scale, translation and rotation.
		XMVECTOR screenPosition = XMLoadFloat3(&pImpl->vertices[i].position);
		screenPosition *= XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_1X>(sourceRectangle, DirectX::g_XMOne)*scale;
		screenPosition += position;
		pImpl->vertices[i].position = Rotation(screenPosition, rotation, origin);
		// To NDC
		pImpl->vertices[i].position.x = 2.0f*pImpl->vertices[i].position.x / viewPort.Width - 1.0f;
		pImpl->vertices[i].position.y = 1.0f - 2.0f*pImpl->vertices[i].position.y / viewPort.Height;

		// Calculate texture coordinate.
		XMVECTOR textureCoordinate = XMLoadFloat2(&pImpl->vertices[i].textureCoordinate);
		textureCoordinate *= XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_1X>(sourceRectangle, DirectX::g_XMOne);
		textureCoordinate += XMVectorPermute<XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1X>(sourceRectangle, DirectX::g_XMZero);
		// To NDC
		textureCoordinate /= XMLoadFloat2(&XMFLOAT2(texture->width, texture->height));
		XMStoreFloat2(&pImpl->vertices[i].textureCoordinate, textureCoordinate);

		// Set vertex color
		pImpl->vertices[i].color = color;
	}

	pImpl->Draw();
}