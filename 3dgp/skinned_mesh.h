#ifndef _SKINEED_MESH_H_
#define _SKINNED_MESH_H_

#include "3dgp_system.h"

#include <vector>


#define MAX_BONE_INFLUENCES (4)
#define MAX_BONES (32)

class SkinnedMesh
{
public:
	struct BoneInfluence
	{
		int index;		// Index of bone
		float weight;	// Weight of bone
	};
	struct Bone
	{
		XMMATRIX transform;
	};

	typedef std::vector<BoneInfluence> BoneInfluencesPerContralPoint;
	typedef std::vector<Bone> Skeletal;

	struct SkeletalAnimation :public std::vector<Skeletal>
	{
		float samplingTime = 1 / 24.0f;
		float animationTick = 0.0f;
		SkeletalAnimation() {
			SkeletalAnimation::vector<Skeletal>(0);
		};
	};

private:
	struct vertex3D
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT4 normal;
		XMFLOAT2 texcoord;
		float boneWeight[MAX_BONE_INFLUENCES] = { 0.0f,0.0f,0.0f,0.0f };
		int boneIndices[MAX_BONE_INFLUENCES] = {};
	};
	struct VS_CBUFFER_PROJECTION
	{
		XMMATRIX world;					//ワールド変換行列
		XMMATRIX view;					//ビュー変換行列
		XMMATRIX projection;			//プロジェクション変換行列
		XMMATRIX worldViewProjection;	//ワールド・ビュー・プロジェクション合成行列
		XMFLOAT4 materialColor;			//材質色
		XMFLOAT4 lightDirection;		//ライト進行行列

		XMMATRIX boneTransforms[MAX_BONES]; //ボーン影響変換行列
	};
	struct Material
	{
		XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView* pShaderResourceView = NULL;
	};
	struct Subset 
	{
		u_int startIndex = 0;	// Start number of index buffers
		u_int IndexCount = 0;	// Number of vertices(indices)
		Material diffuse;
	};
	struct Mesh
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		std::vector<Subset> subsetsList;
		XMMATRIX globalTransform = XMMatrixIdentity();
		// Unit.22
		//std::vector<Bone> skeletal;
		// Unit.23
		SkeletalAnimation skeletalAnimation;
	};

private:
	//ID3D11Buffer*				m_pVertexBuffer;
	//ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11RasterizerState*		m_pWireRasterizerState;
	ID3D11RasterizerState*		m_pFillRasterizerState;
	ID3D11DepthStencilState*	m_pDepthStencilState;

	int m_indexCount;
	int m_vertexCount;
	int m_latitudeNum;
	int m_longitudeNum;

	HRESULT hr;
	//ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11PixelShader*			m_pPixelShader;

	ID3D11SamplerState*			m_pSamplerState;

	//Material m_diffuse;
	//std::vector<Subset> subsetsList;

	std::vector<Mesh> m_meshesList;
	XMMATRIX m_coordinateConversion = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};


public:
	SkinnedMesh(ID3D11Device *a_pDevice, const char *a_pFbxFileName, const bool a_exchangeYandZ = false);
	virtual ~SkinnedMesh();
	
	void createBuffers(ID3D11Device *a_pDevice, ID3D11Buffer** a_ppVertexBuffer, ID3D11Buffer** a_ppIndexBuffer, vertex3D *a_pVertices, int a_vertexNum, WORD *a_pIndices, int a_indexNum);

	inline XMFLOAT3 toNDC(XMFLOAT3 a_input);
	void setProjection(ID3D11DeviceContext *a_pDeviceContext, const Transform& preSetTransform = Transform::initialValue(), const Transform& transform = Transform::initialValue(), const XMMATRIX &a_globalTransform = XMMatrixIdentity(), SkeletalAnimation &a_skeletalAnimation = SkeletalAnimation(), float a_elapsedTime = 1 / 60.0f/*UNIT.23*/);
	void render(ID3D11DeviceContext *a_pDeviceContext, bool a_doFill, const Mesh &a_mesh);

	void drawMesh(ID3D11DeviceContext *a_pDeviceContext, const Transform& preSetTransform = Transform::initialValue(), const Transform& transform = Transform::initialValue(), float elapsedTime = 1 / 60.0f/*UNIT.23*/);
};


#endif // !_SKINEED_MESH_H_
