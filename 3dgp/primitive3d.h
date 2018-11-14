#ifndef _PRIMITIVE3D_H_
#define _PRIMITIVE3D_H_

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

enum GEOMETRY_TYPE
{
	GEOMETRY_CUBE,
	GEOMETRY_CYLINDER,
	GEOMETRY_CIRCLE,
};

class Primitive3D
{
private:
	struct vertex3D
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	};

	struct PROJECTION_CBUFFER
	{
		XMMATRIX world;					//ワールド変換行列
		XMMATRIX view;					//ビュー変換行列
		XMMATRIX projection;			//プロジェクション変換行列
		XMMATRIX worldViewProjection;	//ワールド・ビュー・プロジェクション合成行列
		XMFLOAT4 materialColor;			//材質色
		XMFLOAT4 lightDirection;		//ライト進行行列
	};
private:

	HRESULT hr;
	ID3D11VertexShader*			pVertexShader;
	ID3D11InputLayout*			pInputLayout;
	ID3D11PixelShader*			pPixelShader;
	
	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pIndexBuffer;
	ID3D11Buffer*				pConstantBuffer;
	ID3D11RasterizerState*		pWireRasterizerState;
	ID3D11RasterizerState*		pFillRasterizerState;
	ID3D11DepthStencilState*	pDepthStencilState;
	
	WORD* pIndices;
	UINT indexNum;
	UINT vertexNum;
	UINT latitudeNum;
	UINT longitudeNum;

	template <size_t vertexNum, size_t indexNum>
	void CreateBuffers(ID3D11Device *pDevice, vertex3D(&pVertices)[vertexNum], WORD(&pIndices)[indexNum]) {
		CreateBuffers(pDevice, pVertices, vertexNum, pIndices, indexNum);
	};
	void CreateBuffers(ID3D11Device *pDevice, vertex3D *pVertices, int vertexNum, WORD *pIndices, int indexNum);

	void Draw(ID3D11DeviceContext *pDeviceContext, bool doFill);

public:

	vertex3D *pVertices;

	Primitive3D(ID3D11Device *pDevice);
	virtual ~Primitive3D();

	virtual void Initialize(ID3D11Device *pDevice, const int &type = GEOMETRY_CUBE, const int &latitudeNum = 2, const int &longitudeNum = 6);

	void XM_CALLCONV Draw(ID3D11DeviceContext *pDeviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, FXMVECTOR blendColor = g_XMOne);
};


#endif // !_PRIMITIVE_3D_H_
