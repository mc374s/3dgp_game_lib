#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <d3d11.h>
//#define _XM_NO_INTRINSICS_ 
#include <DirectXMath.h>

namespace GLC {

// Forward declaration
class Texture;

// 
class Sprite
{
private:
	// Constant buffer per object
	struct PROJECTION_CBUFFER
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 worldViewProjection;
		DirectX::XMFLOAT4	materialColor;
		DirectX::XMFLOAT4	lightDirection;
	}updateCbuffer;

private:

	// Private implementation: D3D COM object holder.
	class Impl;
	Impl* pImpl;

	// Forbidden Sprite's copy constructor
	Sprite(const Sprite&) = delete;

public:

	// 
	Sprite(ID3D11DeviceContext* pDeviceContext);
	Sprite(ID3D11DeviceContext* pDeviceContext, char* pFilename, bool doProjection = false);
	~Sprite();

	// Draw sprite with @texture clip by @sourceRectangle in 2D coordinate.
	// @sourceRectangle: 4 elements include top, left, width, height
	// @position, @scale, @rotation, @origin: affine transformation
	// @blendColor: vertex color
	void XM_CALLCONV Draw(Texture* texture,
		DirectX::FXMVECTOR sourceRectangle,
		DirectX::FXMVECTOR position = DirectX::g_XMZero,
		DirectX::FXMVECTOR scale = DirectX::g_XMOne,
		DirectX::CXMVECTOR rotation = DirectX::g_XMZero,
		DirectX::CXMVECTOR origin = DirectX::g_XMZero,
		DirectX::CXMVECTOR blendColor = DirectX::g_XMOne);
	
	// Draw sprite in 3D coordinate.
	void XM_CALLCONV Draw(Texture* texture,
		DirectX::FXMVECTOR sourceRectangle,
		DirectX::FXMMATRIX view,
		DirectX::CXMMATRIX projection,
		DirectX::FXMVECTOR position = DirectX::g_XMZero,
		DirectX::FXMVECTOR scale = DirectX::g_XMOne,
		DirectX::CXMVECTOR rotation = DirectX::g_XMZero,
		DirectX::CXMVECTOR origin = DirectX::g_XMZero,
		DirectX::CXMVECTOR blendColor = DirectX::g_XMOne);

}; // class Sprite

}; // namespace GLC


#endif // ! _SPRITE_H_