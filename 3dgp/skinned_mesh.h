#ifndef _SKINEED_MESH_H_
#define _SKINNED_MESH_H_

#define MAX_BONE_INFLUENCES (4)
#define MAX_BONES (32)

#include "3dgp.h"

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
		SkeletalAnimation skeletalAnimation;
	};

private:
	ID3D11Buffer*				pConstantBuffer;
	ID3D11RasterizerState*		pWireRasterizerState;
	ID3D11RasterizerState*		pFillRasterizerState;
	ID3D11DepthStencilState*	pDepthStencilState;
	ID3D11SamplerState*			pSamplerState;

	int indexCount;
	int vertexCount;
	int latitudeNum;
	int longitudeNum;

	HRESULT hr;
	ID3D11VertexShader*			pVertexShader;
	ID3D11InputLayout*			pInputLayout;
	ID3D11PixelShader*			pPixelShader;


	std::vector<Mesh> meshesList;
	XMMATRIX coordinateConversion = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	void CreateBuffers(ID3D11Device *pDevice, ID3D11Buffer** ppVertexBuffer, ID3D11Buffer** ppIndexBuffer, vertex3D *pVertices, int vertexNum, WORD *pIndices, int indexNum);

public:
	int frame = 0;

	SkinnedMesh(ID3D11Device *pDevice, const char *pFbxFileName, const bool exchangeAxisYwithAxisZ = false);
	virtual ~SkinnedMesh();

	//
	//Set Shaders and States
	//
	void Render(ID3D11DeviceContext *pDeviceContext, bool isWireframe, const Mesh &mesh);

	//
	//param(animationFrame) Set to 0 means always loop this fbx animation
	//param(elapsedTime) How many ms this frame used
	//
	void Draw(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, bool isWireframe = false, const int& animationFrame = 0, float elapsedTime = 1 / 60.0f);
};


#endif // !_SKINEED_MESH_H_
