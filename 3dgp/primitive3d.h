#ifndef _PRIMITIVE3D_H_
#define _PRIMITIVE3D_H_

#include "3dgp_system.h"

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
		//XMFLOAT3 position = { 1.0f,1.0f,1.0f };
		//XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
		//XMFLOAT3 normal = { 1.0f,1.0f,1.0f };
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
	//ID3D11ShaderResourceView*	pShaderResourceView;
	ID3D11VertexShader*			pVertexShader;
	ID3D11InputLayout*			pInputLayout;
	ID3D11PixelShader*			pPixelShader;
	
	ID3D11Buffer*				pVertexBuffer;
	ID3D11Buffer*				pIndexBuffer;
	ID3D11Buffer*				pConstantBuffer;
	ID3D11RasterizerState*		pWireRasterizerState;
	ID3D11RasterizerState*		pFillRasterizerState;
	ID3D11DepthStencilState*	pDepthStencilState;
	
	vertex3D *pVertices;
	WORD *pIndices;
	UINT indexNum;
	UINT vertexNum;
	UINT latitudeNum;
	UINT longitudeNum;

public:
	Primitive3D(ID3D11Device *pDevice);
	virtual ~Primitive3D();

	virtual void initialize(ID3D11Device *pDevice, const int &type = GEOMETRY_CUBE, const int &latitudeNum = 2, const int &longitudeNum = 6);

	template <size_t vertexNum,size_t indexNum>
	void createBuffers(ID3D11Device *pDevice, vertex3D (&pVertices)[vertexNum], WORD (&pIndices)[indexNum]) {
		createBuffers(pDevice, pVertices, vertexNum, pIndices, indexNum);
	};
	void createBuffers(ID3D11Device *pDevice, vertex3D *pVertices, int vertexNum, WORD *pIndices, int indexNum);

	inline XMFLOAT3 toNDC(const XMFLOAT3 &inputCoord);
	void setProjection(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const XMFLOAT4 &materialColor = XMFLOAT4(1, 1, 1, 1), const Transform& transform = Transform::initialValue());
	void render(ID3D11DeviceContext *pDeviceContext, bool doFill);

	void drawCube(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const XMFLOAT3 &size, const UINTCOLOR &blendColor = 0xFFFFFFFF, const Transform& transform = Transform::initialValue());
	void drawCylinder(ID3D11DeviceContext *pDeviceContext, const XMFLOAT3 &position, const XMFLOAT3 &size, const Transform& transform = Transform::initialValue());
};


#endif // !_PRIMITIVE_3D_H_
