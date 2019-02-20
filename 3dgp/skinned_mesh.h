#ifndef _SKINEED_MESH_H_
#define _SKINNED_MESH_H_

#include <d3d11.h>
//#define _XM_NO_INTRINSICS_ 
#include <DirectXMath.h>

#include <vector>

#define MAX_BONE_INFLUENCES (4)
#define MAX_BONES (128)

class SkinnedMesh
{
public:
	struct BoneInfluence
	{
		size_t index;		// Index of bone
		float weight;	// Weight of bone
	};
	struct Bone
	{
		DirectX::XMFLOAT4X4 transform;
		//char				name[256];
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
	struct Vertex3D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 texcoord;
		float boneWeight[MAX_BONE_INFLUENCES] = { 0.0f,0.0f,0.0f,0.0f };
		size_t boneIndices[MAX_BONE_INFLUENCES] = {};
	};
	struct VS_CBUFFER_PROJECTION
	{
		DirectX::XMFLOAT4X4 world;					//ワールド変換行列
		DirectX::XMFLOAT4X4 view;					//ビュー変換行列
		DirectX::XMFLOAT4X4 projection;				//プロジェクション変換行列
		DirectX::XMFLOAT4X4 worldViewProjection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4 materialColor;			//材質色
		DirectX::XMFLOAT4 lightDirection;			//ライト進行行列

		DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]; //ボーン影響変換行列
	}updateCbuffer;
	struct Material
	{
		DirectX::XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView* pShaderResourceView = NULL;
		char textureFileName[256];
	};
	struct Subset 
	{
		size_t startIndex = 0;	// Start number of index buffers
		size_t IndexCount = 0;	// Number of vertices(indices)
		Material diffuse;
	};
	struct Mesh
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		std::vector<Vertex3D> verticesList;
		std::vector<WORD> indicesList;
		std::vector<Subset> subsetsList;
		DirectX::XMFLOAT4X4 globalTransform = {
			1.f,0,0,0,
			0,1.f,0,0,
			0,0,1.f,0,
			0,0,0,1.f };
		SkeletalAnimation skeletalAnimation;
	};

private:

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

	ID3D11Buffer*				pConstantBuffer;


	DirectX::XMFLOAT4X4 coordinateConversion = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	void CreateBuffers(ID3D11Device *pDevice, ID3D11Buffer** ppVertexBuffer, ID3D11Buffer** ppIndexBuffer, Vertex3D *pVertices, int vertexNum, WORD *pIndices, int indexNum);

	int SaveFbxMeshData(const char* pOutputFilePath, std::vector<Mesh>& meshesList);
	int LoadFbxMeshData(ID3D11Device *pDevice, const char* pInputFilePath, std::vector<Mesh>& meshesList);
	int FetchAnimationsWithoutMesh(void* pFbxScene, SkinnedMesh::SkeletalAnimation &skeletalAnimation);

public:
	int frame = 0;
	std::vector<Mesh> meshesList;

	SkinnedMesh(ID3D11Device *pDevice, const char *pFbxFileName, const bool exchangeAxisYwithAxisZ = false, int* maxFrame = nullptr);
	virtual ~SkinnedMesh();

	void SetAnimation(const SkinnedMesh* pSkinnedMesh, int meshNO = 0);

	//
	//Set Shaders and States
	//
	void Draw(ID3D11DeviceContext *pDeviceContext, bool isWireframe, const Mesh &mesh);

	//
	//param(animationFrame) Set to 0 means always loop this fbx animation
	//param(elapsedTime) How many seconds was last frame used
	//
	void XM_CALLCONV Draw(ID3D11DeviceContext *pDeviceContext, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection,
		bool isWireframe = false, int* animationFrame = nullptr, float elapsedTime = 1 / 60.0f);
};


#endif // !_SKINEED_MESH_H_
