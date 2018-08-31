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
	//ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11PixelShader*			m_pPixelShader;
	
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11RasterizerState*		m_pWireRasterizerState;
	ID3D11RasterizerState*		m_pFillRasterizerState;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	
	vertex3D *m_pVertices;
	WORD *m_pIndices;
	UINT m_indexCount;
	UINT m_vertexCount;
	UINT m_latitudeNum;
	UINT m_longitudeNum;

public:
	Primitive3D(ID3D11Device *a_pDevice);
	virtual ~Primitive3D();

	virtual void initialize(ID3D11Device *a_pDevice, const int &a_type = GEOMETRY_CUBE, const int &a_latitudeNum = 2, const int &a_longitudeNum = 6);

	template <size_t a_vertexNum,size_t a_indexNum>
	void createBuffers(ID3D11Device *a_pDevice, vertex3D (&a_pVertices)[a_vertexNum], WORD (&a_pIndices)[a_indexNum]) {
		createBuffers(a_pDevice, a_pVertices, a_vertexNum, a_pIndices, a_indexNum);
	};
	void createBuffers(ID3D11Device *a_pDevice, vertex3D *a_pVertices, int a_vertexNum, WORD *a_pIndices, int a_indexNum);

	inline XMFLOAT3 toNDC(const XMFLOAT3 &a_inputCoord);
	void setProjection(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const CUSTOM3D* a_pCustom3D = nullptr);
	void render(ID3D11DeviceContext *a_pDeviceContext, bool a_doFill);

	void drawCube(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const XMFLOAT3 &a_size,const CUSTOM3D* a_pCustom3D = nullptr);
	void drawCylinder(ID3D11DeviceContext *a_pDeviceContext, const XMFLOAT3 &a_position, const XMFLOAT3 &a_size,const CUSTOM3D* a_pCustom3D = nullptr);
};


#endif // !_PRIMITIVE_3D_H_
