#include "skinned_mesh.h"

#include "resources_manager.h"
#include "state_subset.h"

#include "VertexTypes.h"

//#include <crtdbg.h>
#include <functional>

#pragma comment(lib,"libfbxsdk-md")
#include <fbxsdk.h>

using namespace fbxsdk;

using namespace DirectX;
using namespace GLC;


#define SAFE_WRITE(pBuffer,size,pFile) {if(fwrite(pBuffer,size,1,pFile)<1){_RPT0(_CRT_ERROR, "File write error!\n"); return -1;}}
#define SAFE_READ(pBuffer,size,pFile) {if(fread(pBuffer,size,1,pFile)<1){_RPT0(_CRT_ERROR, "File read error!\n"); return -1;}}


void FetchBoneInfluences(const FbxMesh *pFbxMesh, std::vector<SkinnedMesh::BoneInfluencesPerContralPoint> &influences)
{
	const int controlPointsNum = pFbxMesh->GetControlPointsCount();
	influences.resize(controlPointsNum);

	const int deformersNum = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int deformerIndex = 0; deformerIndex < deformersNum; ++deformerIndex) {
		FbxSkin	*pSkin = static_cast<FbxSkin*>(pFbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));

		const int clustersNum = pSkin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clustersNum; ++clusterIndex) {
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);

			const int controlPointIndicesNum = pCluster->GetControlPointIndicesCount();
			const int* pControlPointIndicesArray = pCluster->GetControlPointIndices();
			const double* pControlPointWeightsArray = pCluster->GetControlPointWeights();

			for (int i = 0; i < controlPointIndicesNum; ++i){
				SkinnedMesh::BoneInfluencesPerContralPoint &influencesPerControlPoint = influences.at(pControlPointIndicesArray[i]);
				SkinnedMesh::BoneInfluence influence;
				influence.index = clusterIndex;
				influence.weight = static_cast<float>(pControlPointWeightsArray[i]);
				influencesPerControlPoint.push_back(influence);
			}
		}

	}

}

void FetchBoneMatrices(FbxMesh* pFbxMesh, std::vector<SkinnedMesh::Bone> &skeletal, FbxTime time)
{
	const int deformersNum = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int deformerIndex = 0; deformerIndex < deformersNum; ++deformerIndex) {
		FbxSkin *pSkin = static_cast<FbxSkin*>(pFbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));

		const int clustersNum = pSkin->GetClusterCount();
		skeletal.resize(clustersNum);
		for (int clusterIndex = 0; clusterIndex < clustersNum; ++clusterIndex) {
			SkinnedMesh::Bone &bone = skeletal.at(clusterIndex);

			FbxCluster *pCluster = pSkin->GetCluster(clusterIndex);
			//
			FbxNode* linked_node = pCluster->GetLink();
			pCluster->GetName();
			//_RPTN(_CRT_WARN, "%s:\n", linked_node->GetName());
			//strcpy_s(bone.name, linked_node->GetName());

			// This matrix transform coordinates of the initial pose from mesh spece to global space
			FbxAMatrix referenceGlobalInitPosition;
			pCluster->GetTransformMatrix(referenceGlobalInitPosition);

			// This matrix transform coordinates of the initial pose from bone space to global space
			FbxAMatrix clusterGlobalInitPosition;
			clusterGlobalInitPosition = pCluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

			// This matrix transform coordinates of the current pose from bone space to global space
			FbxAMatrix clusterGlobalCurrentPosition;
			clusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(time);

			// This matrix transform coordinates of the current pose form mesh space to global space
			FbxAMatrix referenceGlobalCurrentPosition;
			referenceGlobalCurrentPosition = pFbxMesh->GetNode()->EvaluateGlobalTransform(time);

			// Matrices are defined using the Cloum Major scheme. When a FbxAMatrix represents the translation part of the transformation
			FbxAMatrix transform = referenceGlobalCurrentPosition.Inverse() * clusterGlobalCurrentPosition * clusterGlobalInitPosition.Inverse()*referenceGlobalInitPosition;

			// Convert FbxAMatrix(transform) to XMFLOAT4X4(bone.transform)
			bone.transform = XMFLOAT4X4(
				transform.mData[0][0], transform.mData[0][1], transform.mData[0][2], transform.mData[0][3],
				transform.mData[1][0], transform.mData[1][1], transform.mData[1][2], transform.mData[1][3],
				transform.mData[2][0], transform.mData[2][1], transform.mData[2][2], transform.mData[2][3],
				transform.mData[3][0], transform.mData[3][1], transform.mData[3][2], transform.mData[3][3]
			);
				
			/*for (int row = 0, column = 0; row < 4; ++row) {
				for (column = 0; column < 4; ++column) {
					bone.transform(row, column) = static_cast<float>(transform[row][column]);
				}
			}*/
		}
	}
}

void FetchAnimations(FbxMesh *pFbxMesh, SkinnedMesh::SkeletalAnimation &skeletalAnimation, u_int samplingRate = 0)
{
	// Get the list of all the animation stack
	FbxArray<FbxString*> pAnimationStackNamesArray;
	pFbxMesh->GetScene()->FillAnimStackNameArray(pAnimationStackNamesArray);

	// Get the number of animations
	int animationsNum = pAnimationStackNamesArray.Size();

	if (animationsNum > 0) {
		// Get the FbxTime per animation's frame
		FbxTime::EMode timeMode = pFbxMesh->GetScene()->GetGlobalSettings().GetTimeMode();
		FbxTime frameTime;
		frameTime.SetTime(0, 0, 0, 1, 0, timeMode);

		samplingRate = samplingRate > 0 ? samplingRate : (u_int)frameTime.GetFrameRate(timeMode);
		float samplingTime = 1.0f / samplingRate;
		skeletalAnimation.samplingTime = samplingTime;
		skeletalAnimation.animationTick = 0;

		FbxString *animationStackName = pAnimationStackNamesArray.GetAt(0);
		FbxAnimStack *currentAnimationStack = pFbxMesh->GetScene()->FindMember<FbxAnimStack>(animationStackName->Buffer());
		pFbxMesh->GetScene()->SetCurrentAnimationStack(currentAnimationStack);

		FbxTakeInfo *pTakeInfo = pFbxMesh->GetScene()->GetTakeInfo(animationStackName->Buffer());
		FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart();
		FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop();

		FbxTime samplingStep;
		samplingStep.SetTime(0, 0, 1, 0, 0, timeMode);
		samplingStep = static_cast<FbxLongLong>(samplingStep.Get()*samplingTime);
		for (FbxTime currentTime = startTime; currentTime < endTime; currentTime += samplingStep) {
			SkinnedMesh::Skeletal skeletal;
			FetchBoneMatrices(pFbxMesh, skeletal, currentTime);
			skeletalAnimation.push_back(skeletal);
		}
	}
	for (int i = 0; i < animationsNum; ++i) {
		delete pAnimationStackNamesArray[i];
	}

}

SkinnedMesh::SkinnedMesh(ID3D11Device *pDevice, const char *pFbxFileName, const bool exchangeAxisYwithAxisZ, int* maxFrame)
{
	if (!exchangeAxisYwithAxisZ){
		coordinateConversion = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	// Get the Folder path
	int meshFileFloderPathLength = 0;
	//char *pFileFolderPath = new char[strlen(pFbxFileName)];
	char pFileFolderPath[256];
	char pFileName[256];
	for (int i = strlen(pFbxFileName), length = i; i >= 0; --i) {
		if (*(pFbxFileName + i) == '/' || *(pFbxFileName + i) == '\\') {
			meshFileFloderPathLength = i + 1;
			strncpy_s(pFileFolderPath, pFbxFileName, meshFileFloderPathLength);
			//memcpy(pFileFolderPath, pFbxFileName, meshFileFloderPathLength);
			// Memcpy Does not copy the '\0'
			//pFileFolderPath[meshFileFloderPathLength] = '\0';

			strcpy_s(pFileName, pFbxFileName + meshFileFloderPathLength);

			break;
		}
	}
	// Make Output File Path
	char outputFileName[256] = "./Data/";
	//strcpy_s(outputFileName, pFileFolderPath);
	//strcat_s(outputFileName, "fbx_output/");
	strcat_s(outputFileName, pFileName);
	strcat_s(outputFileName, "_output");

	if (LoadFbxMeshData(pDevice, outputFileName,meshesList)==0) {

	}
	else {
		// Create the FBX SDK manager
		FbxManager* pFbxManager = FbxManager::Create();

		// Create an IOSetting object, IOSROOT is defined in Fbxiosettingspath.h
		pFbxManager->SetIOSettings(FbxIOSettings::Create(pFbxManager, IOSROOT));

		// Create an importer
		FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");

		// Initialize the importer
		bool isImportSuccessed = false;
		isImportSuccessed = pFbxImporter->Initialize(pFbxFileName, -1, pFbxManager->GetIOSettings());
		_ASSERT_EXPR(isImportSuccessed, pFbxImporter->GetStatus().GetErrorString());

		// Create a new scene, so it can be populated by the imported file
		FbxScene* scene = FbxScene::Create(pFbxManager, "");

		// Import the contents of the file into the scene
		isImportSuccessed = pFbxImporter->Import(scene);
		_ASSERT_EXPR(isImportSuccessed, pFbxImporter->GetStatus().GetErrorString());

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter geometryConverter(pFbxManager);
		geometryConverter.Triangulate(scene,/*replace*/true);

		// Fetch node attributes and materials under this node recursively. Currently only mesh
		std::vector<FbxNode*> fetchedMeshes;
		std::function<void(FbxNode*)> traverse = [&](FbxNode* node) {
			if (node) {
				FbxNodeAttribute* pFbxNodeAttribute = node->GetNodeAttribute();
				if (pFbxNodeAttribute) {
					switch (pFbxNodeAttribute->GetAttributeType()) {
					case FbxNodeAttribute::eMesh:
						fetchedMeshes.push_back(node);
						break;
					}
				}
				for (int i = 0; i < node->GetChildCount(); i++) {
					traverse(node->GetChild(i));
				}
			}
		};
		traverse(scene->GetRootNode());

		//FbxMesh *pFbxMesh = fetchedMeshes.at(0)->GetMesh();		// Currently only one mesh
		//FbxMesh *pFbxMesh = nullptr;
		meshesList.resize(fetchedMeshes.size());



		if (meshesList.size() == 0) {
			meshesList.resize(1);
			FetchAnimationsWithoutMesh(scene, meshesList[0].skeletalAnimation);
		}



		for (size_t i = 0, max = fetchedMeshes.size(); i < max; i++) {
			FbxMesh *pFbxMesh = fetchedMeshes.at(i)->GetMesh();


			// Fetch Bone Influences
			std::vector<BoneInfluencesPerContralPoint> boneInfluences;
			FetchBoneInfluences(pFbxMesh, boneInfluences);

			Mesh &mesh = meshesList.at(i);
			// Fetch mesh data
			//std::vector<Vertex3D> verticesList;		// As for vertex buffer
			//std::vector<WORD> indicesList;			// As for index buffer
			u_int vertexCount = 0;

			FbxStringList uvNamesList;
			pFbxMesh->GetUVSetNames(uvNamesList);

			const FbxVector4 *pControlPointsArray = pFbxMesh->GetControlPoints();
			const int polygonsNum = pFbxMesh->GetPolygonCount();

			// Fetch Bone transform
			//FbxTime::EMode timeMode = pFbxMesh->GetScene()->GetGlobalSettings().GetTimeMode();
			//FbxTime frameTime;
			//frameTime.SetTime(0, 0, 0, 1, 0, timeMode);
			//fetchBoneMatrices(pFbxMesh, mesh.skeletal, frameTime * 20); // pose at frame 20 for testing

			// Fetch Animations
			FetchAnimations(pFbxMesh, mesh.skeletalAnimation);

			// Fetch global transform matrix
			FbxAMatrix globalTransform = pFbxMesh->GetNode()->EvaluateGlobalTransform(0);
			mesh.globalTransform = XMFLOAT4X4(
				globalTransform.mData[0][0], globalTransform.mData[0][1], globalTransform.mData[0][2], globalTransform.mData[0][3],
				globalTransform.mData[1][0], globalTransform.mData[1][1], globalTransform.mData[1][2], globalTransform.mData[1][3],
				globalTransform.mData[2][0], globalTransform.mData[2][1], globalTransform.mData[2][2], globalTransform.mData[2][3],
				globalTransform.mData[3][0], globalTransform.mData[3][1], globalTransform.mData[3][2], globalTransform.mData[3][3]
			);
			/*for (int row = 0, column = 0; row < 4; ++row) {
			for (column = 0; column < 4; ++column) {
			mesh.globalTransform(row, column) = static_cast<float>(globalTransform[row][column]);
			}
			}*/
			// Fetch material properties
			const int materialsNum = pFbxMesh->GetNode()->GetMaterialCount();
			if (materialsNum > 0) {
				mesh.subsetsList.resize(materialsNum);
			}
			else {
				Subset dummyData;
				RM::LoadShaderResourceView(pDevice, &dummyData.diffuse.pShaderResourceView, nullptr, nullptr);
				strcpy_s(dummyData.diffuse.textureFileName, "null");
				mesh.subsetsList.push_back(dummyData);
			}

			for (int materialsIndex = 0; materialsIndex < materialsNum; materialsIndex++) {
				Subset &subset = mesh.subsetsList.at(materialsIndex);

				const FbxSurfaceMaterial *pSurfaceMaterial = pFbxMesh->GetNode()->GetMaterial(materialsIndex);
				const FbxProperty property = pSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
				const FbxProperty factor = pSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDisplacementFactor);

				if (property.IsValid() && factor.IsValid()) {
					FbxDouble3 color = property.Get<FbxDouble3>();
					double f = factor.Get<FbxDouble>();
					subset.diffuse.color.x = static_cast<float>(color[0] * f);
					subset.diffuse.color.y = static_cast<float>(color[1] * f);
					subset.diffuse.color.z = static_cast<float>(color[2] * f);
					subset.diffuse.color.w = 1.0f;
				}
				if (property.IsValid()) {
					const int texturesNum = property.GetSrcObjectCount<FbxFileTexture>();
					// Save the txtures number

					if (texturesNum > 0) {

						const FbxFileTexture *pTextureFile = property.GetSrcObject<FbxFileTexture>();
						if (pTextureFile) {

							const char *pUVFileName = pTextureFile->GetRelativeFileName();
							// Create "diffuse.pShaderResourceView"

							// Already get the mesh file's floder path
							int uvFileNameLength = strlen(pUVFileName) + 1;
							const int uvFilePathLength = meshFileFloderPathLength + uvFileNameLength;
							char *pUVFilePath = new char[uvFilePathLength];
							memcpy(pUVFilePath, pFileFolderPath, meshFileFloderPathLength);
							pUVFilePath[meshFileFloderPathLength] = '\0';
							strcat_s(pUVFilePath, uvFilePathLength, pUVFileName);

							RM::LoadShaderResourceView(pDevice, &subset.diffuse.pShaderResourceView, pUVFilePath, nullptr);

							strcpy_s(subset.diffuse.textureFileName, pUVFilePath);
							if (pUVFilePath) {
								delete[] pUVFilePath;
							}

						}
					}
					else {
						RM::LoadShaderResourceView(pDevice, &subset.diffuse.pShaderResourceView, nullptr, nullptr);
						strcpy_s(subset.diffuse.textureFileName, "null");
					}

				}

			}



			if (materialsNum > 0)
			{
				// Count the faces of each material
				for (int polygonIndex = 0; polygonIndex < polygonsNum; ++polygonIndex) {
					const u_int materialIndex = pFbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex);
					mesh.subsetsList.at(materialIndex).IndexCount += 3;
				}
				// Record the offset (how manay vertex)
				int offset = 0;
				for (Subset &subset : mesh.subsetsList) {
					subset.startIndex = offset;
					offset += subset.IndexCount;
					// This will be used as counter in the following procedures, reset to zero
					subset.IndexCount = 0;
				}
			}
			mesh.indicesList.resize(polygonsNum * 3);
			int materialIndex = 0;
			for (int polygonIndex = 0; polygonIndex < polygonsNum; ++polygonIndex) {
				// The material for current face
				materialIndex = 0;
				if (materialsNum > 0) {
					materialIndex = pFbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex);
				}
				// Where should I save the vertex attribute index, according to the material

				Subset &subset = mesh.subsetsList.at(materialIndex);
				const int offsetIndex = subset.startIndex + subset.IndexCount;

				for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
					Vertex3D vertex;
					const int controlPointIndex = pFbxMesh->GetPolygonVertex(polygonIndex, vertexIndex);
					vertex.position.x = static_cast<float>(pControlPointsArray[controlPointIndex][0]);
					vertex.position.y = static_cast<float>(pControlPointsArray[controlPointIndex][1]);
					vertex.position.z = static_cast<float>(pControlPointsArray[controlPointIndex][2]);

					FbxVector4 normal;
					pFbxMesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
					vertex.color = subset.diffuse.color;
					// Get UV coordinates
					if (pFbxMesh->GetElementUVCount() > 0) {
						FbxVector2 uv;
						bool isUnmappedUV;
						pFbxMesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNamesList[0], uv, isUnmappedUV);
						vertex.texcoord.x = static_cast<float>(uv[0]);
						vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
					}

					// Set Bone Influences
					for (int i = 0, max = boneInfluences[controlPointIndex].size(); i < max; ++i) {
						vertex.boneWeight[i] = boneInfluences[controlPointIndex][i].weight;
						vertex.boneIndices[i] = boneInfluences[controlPointIndex][i].index;
					}

					mesh.verticesList.push_back(vertex);
					//indicesList.push_back(vertexCount);
					mesh.indicesList.at(offsetIndex + vertexIndex) = static_cast<u_int>(vertexCount);

					vertexCount += 1;


				}
				subset.IndexCount += 3;
			}
			// Create vertex, index, constant buffers
			/*vertexCount = mesh.verticesList.size();
			indexCount = mesh.indicesList.size();
			Vertex3D *vertices = new Vertex3D[vertexCount];
			WORD *indices = new WORD[indexCount];

			for (u_int i = 0; i < vertexCount; ++i)
			{
			vertices[i] = mesh.verticesList[i];
			}
			for (int i = 0; i < indexCount; i++)
			{
			indices[i] = mesh.indicesList[i];
			}*/

			//CreateBuffers(pDevice, &mesh.vertexBuffer, &mesh.indexBuffer, vertices, vertexCount, indices, indexCount);
			CreateBuffers(pDevice, &mesh.vertexBuffer, &mesh.indexBuffer,
				mesh.verticesList.data(), mesh.verticesList.size(), mesh.indicesList.data(), mesh.indicesList.size());
			//delete vertices;
			//delete indices;
		}

		//SAFE_DELETE(pFileFolderPath);
		pFbxManager->Destroy();


		// TODO: fwrite, fread;
		SaveFbxMeshData(outputFileName, meshesList);

	}
	if (maxFrame) {
		(*maxFrame) = meshesList[0].skeletalAnimation.size();
	}

	
	// Initailize DirectX3D COM objects

	// Create constant buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VS_CBUFFER_PROJECTION);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	
	VS_CBUFFER_PROJECTION VSConstantData;
	//ZeroMemory(&VSConstantData, sizeof(VS_CBUFFER_PROJECTION));


	hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pConstantBuffer failed", L"SkinnedMesh::CreateBuffers()", 0);
		exit(-1);
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",		0, DXGI_FORMAT_R32G32B32A32_SINT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RM::LoadVertexShader(pDevice, "./Data/Shaders/texture_on_3d_bone_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);

	//RM::LoadVertexShader(pDevice, "./Data/Shaders/common_vs_diffuse_tex_normal_bone.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);

	RM::LoadPixelShader(pDevice, "./Data/Shaders/common_ps_diffuse_tex.cso", &pPixelShader);

	//VertexPositionNormalTangentColorTextureSkinning vertex;
	//vertex.weights;
	//sizeof(VertexPositionNormalTangentColorTextureSkinning);
	//sizeof(Vertex3D);
	//VertexPositionNormalColorTexture pnct;
	//VertexPos

	// Create sampler state reference
	pSamplerState = Sampler::State(Sampler::WRAP);

	// Create rasterizer state reference
	pFillRasterizerState = Rasterizer::State::Get(Rasterizer::CULL_COUNTER_CLOCKWISE);
	pWireRasterizerState = Rasterizer::State::Get(Rasterizer::WIREFRAME);

	// Create depth stencil state reference
	pDepthStencilState = DepthStencil::State::Get(DepthStencil::DEFAULT);


}

void SkinnedMesh::CreateBuffers(ID3D11Device *pDevice, ID3D11Buffer** ppVertexBuffer, ID3D11Buffer** ppIndexBuffer, Vertex3D *pVertices, int vertexNum, WORD *pIndices, int indexNum)
{
	//pVertices = new Vertex3D[_vertexNum];
	//memcpy(pVertices, _vertices, sizeof(Vertex3D)*_vertexNum);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT/*D3D11_USAGE_DYNAMIC*/;
	bufferDesc.ByteWidth = sizeof(Vertex3D)*(vertexNum + 1);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_WRITE*/0;

	// Create vertex buffer
	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = pVertices;
	hr = pDevice->CreateBuffer(&bufferDesc, &subResourceData, ppVertexBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, L"Create pVertexBuffer failed", L"SkinnedMesh::CreateBuffers()", 0);
		exit(-1);
	}
	
	//pIndices = new UINT[_indexNum];
	//memcpy(pIndices, _indices, sizeof(UINT)*_indexNum);
	// Create index buffer
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(WORD)*(indexNum);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bufferDesc.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = pIndices;
	hr = pDevice->CreateBuffer(&bufferDesc, &subResourceData, ppIndexBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pIndexBuffer failed", L"SkinnedMesh::CreateBuffers()", 0);
		exit(-1);
	}
}

int SkinnedMesh::FetchAnimationsWithoutMesh(void* pFbxScene, SkinnedMesh::SkeletalAnimation &skeletalAnimation)
{
	FbxScene* scene = (FbxScene*)pFbxScene;

	//FbxArray<FbxString*> pAnimationStackNamesArray;
	//scene->FillAnimStackNameArray(pAnimationStackNamesArray);
	FbxAnimStack* pAnimStack = scene->GetCurrentAnimationStack();
	FbxTimeSpan timeSpan = pAnimStack->GetLocalTimeSpan();
	FbxTime animDuration = timeSpan.GetDuration();
	FbxTime::EMode timeMode = scene->GetGlobalSettings().GetTimeMode();
	int frameNum = animDuration.GetFrameCount(timeMode);
	int boneNum = scene->GetNodeCount() - 1;
	char** boneNames;
	boneNames = new char*[boneNum];
	for (int i = 0; i < boneNum; ++i) {
		boneNames[i] = new char[128];
		strcpy_s(boneNames[i], 128, scene->GetNode(i + 1)->GetName());
	}

	//Mesh meshTemp;
	//meshTemp.skeletalAnimation.resize(0);

	FbxTime startTime = timeSpan.GetStart();
	FbxTime endTime = timeSpan.GetStop();
	FbxTime frameTime;
	frameTime.SetTime(0, 0, 0, 1, 0, timeMode);
	u_int samplingRate = 0;
	samplingRate = samplingRate > 0 ? samplingRate : (u_int)frameTime.GetFrameRate(timeMode);
	float samplingTime = 1.0f / samplingRate;
	FbxTime samplingStep;
	samplingStep.SetTime(0, 0, 1, 0, 0, timeMode);
	samplingStep = static_cast<FbxLongLong>(samplingStep.Get()*samplingTime);
	skeletalAnimation.samplingTime = samplingTime;
	skeletalAnimation.animationTick = 0;

	FbxNode* root = scene->GetRootNode();
	//FbxMesh* pFbxMesh = static_cast<FbxMesh*>(root->GetChild(1)->GetMesh());
	//FetchAnimations(pFbxMesh, skeletalAnimation);

	for (FbxTime currentTime = startTime; currentTime < endTime; currentTime += samplingStep) {
		Skeletal skeletal;
		skeletal.resize(boneNum);
		for (int j = 0; j < boneNum; ++j) {
			SkinnedMesh::Bone &bone = skeletal.at(j);
			//FbxNode* pNode = root->FindChild(boneNames[j]);
			FbxNode* pNode = scene->GetNode(j + 1);
			pNode->FindDstObject(0);
			FbxNodeAttribute* pFbxNodeAttribute = pNode->GetNodeAttribute();
			if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton) {
				FbxSkeleton* pFbxSkeleton = pNode->GetSkeleton();
				//pFbxSkeleton->
				
				//_RPTN(_CRT_WARN, "%s:\n", linked_node->GetName());
				//strcpy_s(bone.name, linked_node->GetName());

				// This matrix transform coordinates of the initial pose from mesh spece to global space
				FbxAMatrix referenceGlobalInitPosition;

				// This matrix transform coordinates of the initial pose from bone space to global space
				FbxAMatrix clusterGlobalInitPosition;

				// This matrix transform coordinates of the current pose from bone space to global space
				FbxAMatrix clusterGlobalCurrentPosition;

				// This matrix transform coordinates of the current pose form mesh space to global space
				FbxAMatrix referenceGlobalCurrentPosition;
				referenceGlobalCurrentPosition = pNode->EvaluateGlobalTransform(currentTime);
				//referenceGlobalCurrentPosition = pNode->EvaluateGlobalTransform(currentTime);
				//FbxSkeleton* pSkeleton = pNode->GetSkeleton();
				//FbxAnimEvaluator* pFbxAnimEvaluator = pNode->GetAnimationEvaluator();

				// Matrices are defined using the Cloum Major scheme. When a FbxAMatrix represents the translation part of the transformation
				FbxAMatrix transform = referenceGlobalCurrentPosition.Inverse();
				FbxAMatrix parantTransform = pNode->GetParent()->EvaluateGlobalTransform(currentTime);
				//transform= parantTransform*pNode->LclTranslation.Get()*pNode->RotationOffset*pNode->RotationPivot*pNode->PreRotation*pNode->LclRotation

				// Convert FbxAMatrix(transform) to XMFLOAT4X4(bone.transform)
				bone.transform = XMFLOAT4X4(
					transform.mData[0][0], transform.mData[0][1], transform.mData[0][2], transform.mData[0][3],
					transform.mData[1][0], transform.mData[1][1], transform.mData[1][2], transform.mData[1][3],
					transform.mData[2][0], transform.mData[2][1], transform.mData[2][2], transform.mData[2][3],
					transform.mData[3][0], transform.mData[3][1], transform.mData[3][2], transform.mData[3][3]
				);
			}

		}

		skeletalAnimation.push_back(skeletal);

	}

	//meshesList[0].skeletalAnimation.resize(frameNum);
	//memcpy_s(meshesList[0].skeletalAnimation.data(), sizeof(Skeletal)*frameNum, meshTemp.skeletalAnimation.data(), sizeof(Skeletal)*frameNum);
	if (boneNames) {
		for (int i = 0; i < boneNum; ++i) {
			if (boneNames[i]) {
				delete[] boneNames[i];
			}
		}
		delete[] boneNames;
	}


	//const FbxProperty property = pAnimCurve->FindProperty(FbxAnimCurve::);


	//Mesh meshTemp;
	//// Get the list of all the animation stack
	//FbxArray<FbxString*> pAnimationStackNamesArray;
	//scene->FillAnimStackNameArray(pAnimationStackNamesArray);
	//FbxAnimEvaluator* pFbxAnimEvaluator = scene->GetAnimationEvaluator();
	//pFbxAnimEvaluator->GetNodeGlobalTransform(0);


	// Get the number of animations
	//int animationsNum = pAnimationStackNamesArray.Size();
	//FetchAnimations(scene, meshTemp.skeletalAnimation);
	return 0;
}

void SkinnedMesh::SetAnimation(const SkinnedMesh* pSkinnedMesh, int meshNO)
{
	if (meshesList.size() > meshNO) {
		size_t frameNum = pSkinnedMesh->meshesList[meshNO].skeletalAnimation.size();
		
		meshesList[meshNO].skeletalAnimation.clear();
		meshesList[meshNO].skeletalAnimation.resize(frameNum);
		meshesList[meshNO].skeletalAnimation.animationTick = pSkinnedMesh->meshesList[meshNO].skeletalAnimation.animationTick;
		meshesList[meshNO].skeletalAnimation.samplingTime = pSkinnedMesh->meshesList[meshNO].skeletalAnimation.samplingTime;
		//memcpy_s(meshesList[meshNO].skeletalAnimation.data(), sizeof(XMFLOAT4X4)*bonesNum*frameNum, pSkinnedMesh->meshesList[meshNO].skeletalAnimation.data(), sizeof(XMFLOAT4X4)*bonesNum*frameNum);
		for (size_t frameCount = 0; frameCount < frameNum; ++frameCount) {
			size_t bonesNum = pSkinnedMesh->meshesList[meshNO].skeletalAnimation[frameCount].size();
			meshesList[meshNO].skeletalAnimation[frameCount].resize(bonesNum);
			memcpy_s(meshesList[meshNO].skeletalAnimation[frameCount].data(), sizeof(XMFLOAT4X4)*bonesNum, 
				pSkinnedMesh->meshesList[meshNO].skeletalAnimation[frameCount].data(), sizeof(XMFLOAT4X4)*bonesNum);
		}

	}
}

int SkinnedMesh::SaveFbxMeshData(const char* pOutputFilePath, std::vector<Mesh>& meshesList)
{
	FILE *pFile;

	char outputFileName[256];
	strcpy_s(outputFileName, pOutputFilePath);

	//////////////////////////////////////////////////////////////////////////////////////////////
	//書き出し
	if (fopen_s(&pFile, outputFileName, "wb") != 0) {
		_RPT0(_CRT_ERROR, "File open error!\n");
		return -1;
	}

	size_t meshNum = meshesList.size();
	SAFE_WRITE(&meshNum, sizeof(size_t), pFile);
	for (Mesh& mesh : meshesList) {
		// globalTransform を書き出し
		SAFE_WRITE(&mesh.globalTransform, sizeof(XMFLOAT4X4), pFile);

		// verticesList を書き出し
		size_t verticesNum = mesh.verticesList.size();
		SAFE_WRITE(&verticesNum, sizeof(size_t), pFile);
		if (verticesNum > 0) {
			SAFE_WRITE(mesh.verticesList.data(), sizeof(Vertex3D)*verticesNum, pFile);
		}

		// indeicesList を書き出し
		size_t indicesNum = mesh.indicesList.size();
		SAFE_WRITE(&indicesNum, sizeof(size_t), pFile);
		if (indicesNum>0) {
			SAFE_WRITE(mesh.indicesList.data(), sizeof(WORD)*indicesNum, pFile);
		}

		// subsetsList を書き出し
		size_t subsetsNum = mesh.subsetsList.size();
		SAFE_WRITE(&subsetsNum, sizeof(size_t), pFile);
		for (Subset& subset : mesh.subsetsList) {
			SAFE_WRITE(&subset.startIndex, sizeof(size_t), pFile);
			SAFE_WRITE(&subset.IndexCount, sizeof(size_t), pFile);
			SAFE_WRITE(&subset.diffuse.color, sizeof(XMFLOAT4), pFile);
			SAFE_WRITE(&subset.diffuse.textureFileName, sizeof(subset.diffuse.textureFileName), pFile);
		}

		// skeletalAnimation を書き出し
		size_t framesNum = mesh.skeletalAnimation.size();
		SAFE_WRITE(&framesNum, sizeof(size_t), pFile);
		SAFE_WRITE(&mesh.skeletalAnimation.samplingTime, sizeof(float), pFile);
		SAFE_WRITE(&mesh.skeletalAnimation.animationTick, sizeof(float), pFile);
		for (Skeletal& skeletal : mesh.skeletalAnimation) {
			size_t bonesNum = skeletal.size();
			SAFE_WRITE(&bonesNum, sizeof(size_t), pFile);
			SAFE_WRITE(skeletal.data(), sizeof(XMFLOAT4X4)*bonesNum, pFile);
		}

	}

	fclose(pFile);
	return 0;
}
int SkinnedMesh::LoadFbxMeshData(ID3D11Device *pDevice, const char* pInputFilePath, std::vector<Mesh>& meshesList)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	//読み込み
	FILE *pFile;
	if (fopen_s(&pFile, pInputFilePath, "rb") != 0)
	{
		_RPT0(_CRT_WARN, "File not exit!\n");
		return -1;
	}

	// meshesListのサイズを読み込みして作成
	size_t meshNum;
	SAFE_READ(&meshNum, sizeof(size_t), pFile);
	meshesList.resize(meshNum);
	for (Mesh& mesh : meshesList) {
		// globalTransform を読み込み
		SAFE_READ(&mesh.globalTransform, sizeof(XMFLOAT4X4), pFile);

		// verticesList のサイズを読み込みして作成
		size_t verticesNum;
		SAFE_READ(&verticesNum, sizeof(size_t), pFile);
		mesh.verticesList.resize(verticesNum);
		if (verticesNum > 0) {
			SAFE_READ(mesh.verticesList.data(), sizeof(Vertex3D)*verticesNum, pFile);
		}
		//Vertex3D* vertices = new Vertex3D[verticesNum];
		//SAFE_READ(vertices, sizeof(Vertex3D)*verticesNum, pFile);
		//std::vector<Vertex3D> verticesList(vertices, vertices + verticesNum);
		//mesh.verticesList.swap(verticesList);
		//delete vertices;
		
		// indicesList のサイズを読み込みして作成
		size_t indicesNum;
		SAFE_READ(&indicesNum, sizeof(size_t), pFile);
		mesh.indicesList.resize(indicesNum);
		if (indicesNum > 0) {
			SAFE_READ(mesh.indicesList.data(), sizeof(WORD)*indicesNum, pFile);
		}
		//WORD* indices = new WORD[indicesNum];
		//SAFE_READ(indices, sizeof(WORD)*indicesNum, pFile);
		//std::vector<WORD> indicesList(indices, indices + indicesNum);
		//mesh.indicesList.swap(indicesList);
		//delete indices;

		// バッファーの作成
		if (verticesNum > 0 && indicesNum > 0) {
			CreateBuffers(pDevice, &mesh.vertexBuffer, &mesh.indexBuffer,
				mesh.verticesList.data(), mesh.verticesList.size(), mesh.indicesList.data(), mesh.indicesList.size());
		}

		// subsetsList のサイズを読み込みして作成
		size_t subsetsNum = mesh.subsetsList.size();
		SAFE_READ(&subsetsNum, sizeof(size_t), pFile);
		mesh.subsetsList.resize(subsetsNum);
		for (Subset& subset : mesh.subsetsList) {
			SAFE_READ(&subset.startIndex, sizeof(size_t), pFile);
			SAFE_READ(&subset.IndexCount, sizeof(size_t), pFile);
			SAFE_READ(&subset.diffuse.color, sizeof(XMFLOAT4), pFile);
			SAFE_READ(&subset.diffuse.textureFileName, sizeof(subset.diffuse.textureFileName), pFile);
			// シェーダーリソースビューを作成
			if (strcmp(subset.diffuse.textureFileName, "null") == 0) {
				RM::LoadShaderResourceView(pDevice, &subset.diffuse.pShaderResourceView, nullptr, nullptr);
			}
			else {
				RM::LoadShaderResourceView(pDevice, &subset.diffuse.pShaderResourceView, subset.diffuse.textureFileName, nullptr);
			}
		}

		// skeletalAnimation のサイズを読み込みして作成
		size_t framesNum;
		SAFE_READ(&framesNum, sizeof(size_t), pFile);
		mesh.skeletalAnimation.resize(framesNum);
		SAFE_READ(&mesh.skeletalAnimation.samplingTime, sizeof(float), pFile);
		SAFE_READ(&mesh.skeletalAnimation.animationTick, sizeof(float), pFile);
		for (Skeletal& skeletal : mesh.skeletalAnimation) {
			size_t bonesNum;
			SAFE_READ(&bonesNum, sizeof(size_t), pFile);
			skeletal.resize(bonesNum);
			SAFE_READ(skeletal.data(), sizeof(XMFLOAT4X4)*bonesNum, pFile);
			//XMFLOAT4X4* transforms = new XMFLOAT4X4[bonesNum];
			//SAFE_READ(transforms, sizeof(XMFLOAT4X4)*bonesNum, pFile);
			//std::vector<Bone> bonesList(transforms, transforms + bonesNum);
			//skeletal.swap(bonesList);
			//delete transforms;
		}

	}

	fclose(pFile);

	return 0;
}

void SkinnedMesh::Draw(ID3D11DeviceContext *pDeviceContext, bool isWireframe, const Mesh &mesh)
{
	if (isWireframe) {
		pDeviceContext->RSSetState(pWireRasterizerState);
	}
	else {
		pDeviceContext->RSSetState(pFillRasterizerState);
	}

	UINT pStrides = sizeof(Vertex3D);
	UINT pOffsets = 0;
	pDeviceContext->IASetPrimitiveTopology(/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST/*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	pDeviceContext->VSSetConstantBuffers(3, 1, &pConstantBuffer);
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	//pDeviceContext->PSSetShaderResources(0, 1, &subsetsList[0].diffuse.pShaderResourceView);
	pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	//pDeviceContext->DrawIndexed(indexCount, 0, 0);
	pDeviceContext->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &pStrides, &pOffsets);
	pDeviceContext->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	for each (Subset subset in mesh.subsetsList) {
		pDeviceContext->PSSetShaderResources(0, 1, &subset.diffuse.pShaderResourceView);
		pDeviceContext->DrawIndexed(subset.startIndex + subset.IndexCount, 0, 0);
	}
}


void XM_CALLCONV SkinnedMesh::Draw(ID3D11DeviceContext *pDeviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, bool isWireframe, int* animationFrame, float elapsedTime)
{
	XMMATRIX worldViewProjection(world);
	worldViewProjection *= view;
	worldViewProjection *= projection;

	XMStoreFloat4x4(&updateCbuffer.view, view);
	XMStoreFloat4x4(&updateCbuffer.projection, projection);
	//static XMVECTOR lightDirection = { 0.0f,0.0f,1.0f,0.0f };
	//lightDirection = Camera::mainCamera.focusPosition - Camera::mainCamera.eyePosition;
	updateCbuffer.lightDirection = { XMVectorGetZ(view.r[0]), XMVectorGetZ(view.r[1]), XMVectorGetZ(view.r[2]), 1 };
	updateCbuffer.materialColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Play Fbx Animation	
	XMMATRIX tansformation;
	
	SkeletalAnimation* skeletalAnimation;

	for(Mesh& mesh: meshesList)
	{
		skeletalAnimation = &mesh.skeletalAnimation;
		if (skeletalAnimation->size() > 0) {
			if (animationFrame && (*animationFrame)== 0)
			{
				frame = skeletalAnimation->animationTick / skeletalAnimation->samplingTime;
			}
			else if (animationFrame)
			{
				frame = *animationFrame;
			}

			if (frame > skeletalAnimation->size() - 1) {
				frame = 0;
				if (animationFrame) {
					(*animationFrame) = -1;
				}
				skeletalAnimation->animationTick = 0;
			}
			Skeletal& skeletal = skeletalAnimation->at(frame);
			size_t bonesNum = skeletal.size();
			_ASSERT_EXPR(bonesNum < MAX_BONES, L"The bonesNum exceeds MAX_BONES!");
			for (size_t i = 0; i < bonesNum; i++) {
				//updateCbuffer.boneTransforms[i] = skeletal->at(i).transform;
				memcpy(&updateCbuffer.boneTransforms[i], &skeletal[i].transform, sizeof(XMFLOAT4X4));
			}
			skeletalAnimation->animationTick += elapsedTime;
		}
		tansformation = XMLoadFloat4x4(&mesh.globalTransform)*XMLoadFloat4x4(&coordinateConversion);
		XMStoreFloat4x4(&updateCbuffer.world, tansformation*world);
		XMStoreFloat4x4(&updateCbuffer.worldViewProjection, tansformation*worldViewProjection);

		pDeviceContext->UpdateSubresource(pConstantBuffer, 0, NULL, &updateCbuffer, 0, 0);

		Draw(pDeviceContext, isWireframe, mesh);
	}

}


SkinnedMesh::~SkinnedMesh()
{
	for (Mesh &mesh : meshesList)
	{
		SAFE_RELEASE(mesh.vertexBuffer);
		SAFE_RELEASE(mesh.indexBuffer);
		for (Subset &subset : mesh.subsetsList) {
			RM::ReleaseShaderResourceView(subset.diffuse.pShaderResourceView);
		}
	}

	SAFE_RELEASE(pConstantBuffer);

	pWireRasterizerState = nullptr;
	pFillRasterizerState = nullptr;
	pDepthStencilState = nullptr;

	RM::ReleasePixelShader(pPixelShader);
	RM::ReleaseVertexShader(pVertexShader, pInputLayout);

	SAFE_RELEASE(pSamplerState);

}