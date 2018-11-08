﻿#include "skinned_mesh.h"
#include <fbxsdk.h>
using namespace fbxsdk;


#include <functional>
#include "resources_manager.h"

void fetchBoneInfluences(const FbxMesh *pFbxMesh, std::vector<SkinnedMesh::BoneInfluencesPerContralPoint> &influences)
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

void fetchBoneMatrices(FbxMesh* pFbxMesh, std::vector<SkinnedMesh::Bone> &skeletal, FbxTime time)
{
	const int deformersNum = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int deformerIndex = 0; deformerIndex < deformersNum; ++deformerIndex) {
		FbxSkin *pSkin = static_cast<FbxSkin*>(pFbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));

		const int clustersNum = pSkin->GetClusterCount();
		skeletal.resize(clustersNum);
		for (int clusterIndex = 0; clusterIndex < clustersNum; ++clusterIndex) {
			SkinnedMesh::Bone &bone = skeletal.at(clusterIndex);

			FbxCluster *pCluster = pSkin->GetCluster(clusterIndex);

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
			for (int row = 0, column = 0; row < 4; ++row) {
				for (column = 0; column < 4; ++column) {
					bone.transform(row, column) = static_cast<float>(transform[row][column]);
				}
			}
		}
	}
}

void fetchAnimations(FbxMesh *pFbxMesh, SkinnedMesh::SkeletalAnimation &skeletalAnimation, u_int samplingRate = 0)
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

		samplingRate = samplingRate > 0 ? samplingRate : frameTime.GetFrameRate(timeMode);
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
			fetchBoneMatrices(pFbxMesh, skeletal, currentTime);
			skeletalAnimation.push_back(skeletal);
		}
	}
	for (int i = 0; i < animationsNum; ++i) {
		delete pAnimationStackNamesArray[i];
	}

}

SkinnedMesh::SkinnedMesh(ID3D11Device *pDevice, const char *pFbxFileName, const bool exchangeYandZ)
{
	if (!exchangeYandZ){
		coordinateConversion = XMMatrixIdentity();
	}

	// Create the FBX SDK manager
	FbxManager* pFbxManager = FbxManager::Create();

	// Create an IOSetting object, IOSROOT is defined in Fbxiosettingspath.h
	pFbxManager->SetIOSettings(FbxIOSettings::Create(pFbxManager, IOSROOT));

	// Create an importer
	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");

	// Initialize the importer
	bool isImportSuccessed = false;
	isImportSuccessed = pFbxImporter->Initialize(pFbxFileName, -1, pFbxManager->GetIOSettings());
	_ASSERT_EXPR_A(isImportSuccessed, pFbxImporter->GetStatus().GetErrorString());

	// Create a new scene, so it can be populated by the imported file
	FbxScene* scene = FbxScene::Create(pFbxManager, "");

	// Import the contents of the file into the scene
	isImportSuccessed = pFbxImporter->Import(scene);
	_ASSERT_EXPR_A(isImportSuccessed, pFbxImporter->GetStatus().GetErrorString());

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

	// Get the Folder path
	int meshFileFloderPathLength = 0;
	char *pFileFolderPath = new char[strlen(pFbxFileName)];
	for (int i = strlen(pFbxFileName); i >= 0; --i) {
		if (*(pFbxFileName + i) == '/' || *(pFbxFileName + i) == '\\') {
			meshFileFloderPathLength = i + 1;
			memcpy(pFileFolderPath, pFbxFileName, meshFileFloderPathLength);
			// Memcpy Does not copy the '\0'
			pFileFolderPath[meshFileFloderPathLength] = '\0';
			break;
		}
	}

	//FbxMesh *pFbxMesh = fetchedMeshes.at(0)->GetMesh();		// Currently only one mesh
	//FbxMesh *pFbxMesh = nullptr;
	meshesList.resize(fetchedMeshes.size());
	for (size_t i = 0, max = fetchedMeshes.size(); i < max; i++) {
		FbxMesh *pFbxMesh = fetchedMeshes.at(i)->GetMesh();


		// Fetch Bone Influences
		std::vector<BoneInfluencesPerContralPoint> boneInfluences;
		fetchBoneInfluences(pFbxMesh, boneInfluences);

		Mesh &mesh = meshesList.at(i);
		// Fetch mesh data
		std::vector<vertex3D> verticesList;		// As for vertex buffer
		std::vector<WORD> indicesList;			// As for index buffer
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
		fetchAnimations(pFbxMesh, mesh.skeletalAnimation);

		// Fetch global transform matrix
		FbxAMatrix globalTransform = pFbxMesh->GetNode()->EvaluateGlobalTransform(0);
		for (int row = 0, column = 0; row < 4; ++row) {
			for (column = 0; column < 4; ++column) {
				mesh.globalTransform(row, column) = static_cast<float>(globalTransform[row][column]);
			}
		}
		// Fetch material properties
		const int materialsNum = pFbxMesh->GetNode()->GetMaterialCount();
		if (materialsNum > 0) {
			mesh.subsetsList.resize(materialsNum);
		}
		else {
			Subset dummyData;
			RM::MakeDummyShaderResourceView(pDevice, &dummyData.diffuse.pShaderResourceView);
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
						ID3D11Resource* pResource = NULL;

						RM::loadShaderResourceView(pDevice, pUVFilePath, &pResource, &subset.diffuse.pShaderResourceView);
						SAFE_RELEASE(pResource);
						SAFE_DELETE(pUVFilePath);

						// SAMPLER_DESC Initialize
						D3D11_SAMPLER_DESC samplerDesc;
						ZeroMemory(&samplerDesc, sizeof(samplerDesc));
						//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
						//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
						//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
						//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
						//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
						samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
						samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
						samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
						samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
						samplerDesc.MipLODBias = 0.0f;
						samplerDesc.MaxAnisotropy = 16;
						samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
						/*samplerDesc.BorderColor[0] = 1.0f;
						samplerDesc.BorderColor[1] = 1.0f;
						samplerDesc.BorderColor[2] = 1.0f;
						samplerDesc.BorderColor[3] = 1.0f;*/
						samplerDesc.MinLOD = 0;
						samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

						hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
						if (FAILED(hr))
						{
							MessageBox(0, L"skinned_mesh: Create SamplerState failed", 0, 0);
							exit(-1);
						}

					}
				}
				else {
					RM::MakeDummyShaderResourceView(pDevice, &subset.diffuse.pShaderResourceView);
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
		indicesList.resize(polygonsNum * 3);
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
				vertex3D vertex;
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
				if (pFbxMesh->GetElementUVCount() > 0)
				{
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

				verticesList.push_back(vertex);
				//indicesList.push_back(vertexCount);
				indicesList.at(offsetIndex + vertexIndex) = static_cast<u_int>(vertexCount);

				vertexCount += 1;


			}
			subset.IndexCount += 3;
		}
		// Create vertex, index, constant buffers
		vertexCount = verticesList.size();
		indexCount = indicesList.size();
		vertex3D *vertices = new vertex3D[vertexCount];
		WORD *indices = new WORD[indexCount];

		for (int i = 0; i < vertexCount; i++)
		{
			vertices[i] = verticesList[i];
			//vertices[i].position = toNDC(vertices[i].position);
			//vertices[i].color = diffuse.color;
		}
		for (int i = 0; i < indexCount; i++)
		{
			indices[i] = indicesList[i];
		}

		createBuffers(pDevice, &mesh.vertexBuffer, &mesh.indexBuffer, vertices, vertexCount, indices, indexCount);
		delete vertices;
		delete indices;
	}


	SAFE_DELETE(pFileFolderPath);
	pFbxManager->Destroy();

	// Initailize DirectX3D COM objects

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,			0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,			0, 44,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0, 52,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",		0, DXGI_FORMAT_R32G32B32A32_SINT,		0, 68,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "BONES",		0, DXGI_FORMAT_R32G32B32A32_SINT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RM::loadVertexShader(pDevice, "Data/Shader/texture_on_3d_bone_vs.cso", layoutDesc, ARRAYSIZE(layoutDesc), &pVertexShader, &pInputLayout);

	RM::loadPixelShader(pDevice, "Data/Shader/texture_on_ps.cso", &pPixelShader);

	// Create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	if (exchangeYandZ){
		rasterizerDesc.CullMode = /*D3D11_CULL_BACK*//*D3D11_CULL_NONE*/D3D11_CULL_FRONT;
	}
	else {
		rasterizerDesc.CullMode = /*D3D11_CULL_BACK*/D3D11_CULL_NONE/*D3D11_CULL_FRONT*/;
	}
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pFillRasterizerState);
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pWireRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create RasterizerState failed", L"SkinnedMesh::SkinnedMesh()", 0);
		exit(-1);
	}

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
}

void SkinnedMesh::createBuffers(ID3D11Device *pDevice, ID3D11Buffer** ppVertexBuffer, ID3D11Buffer** ppIndexBuffer, vertex3D *pVertices, int vertexNum, WORD *pIndices, int indexNum)
{
	//pVertices = new vertex3D[_vertexNum];
	//memcpy(pVertices, _vertices, sizeof(vertex3D)*_vertexNum);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = /*D3D11_USAGE_IMMUTABLE*/D3D11_USAGE_DEFAULT/*D3D11_USAGE_DYNAMIC*/;
	bufferDesc.ByteWidth = sizeof(vertex3D)*(vertexNum + 1);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_WRITE*/0;

	// Create vertex buffer
	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = pVertices;
	hr = pDevice->CreateBuffer(&bufferDesc, &subResourceData, ppVertexBuffer);

	if (FAILED(hr))
	{
		MessageBox(0, L"Create pVertexBuffer failed", L"SkinnedMesh::createBuffers()", 0);
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
		MessageBox(0, L"Create pIndexBuffer failed", L"SkinnedMesh::createBuffers()", 0);
		exit(-1);
	}

	// Create constant buffer
	VS_CBUFFER_PROJECTION VSConstantData;
	//ZeroMemory(&VSConstantData, sizeof(VS_CBUFFER_PROJECTION));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VS_CBUFFER_PROJECTION);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = pDevice->CreateBuffer(&bufferDesc, NULL, &pConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"Create pConstantBuffer failed", L"SkinnedMesh::createBuffers()", 0);
		exit(-1);
	}
	
}

void SkinnedMesh::render(ID3D11DeviceContext *pDeviceContext, bool doFill, const Mesh &mesh)
{
	if (doFill) {
		pDeviceContext->RSSetState(pFillRasterizerState);
	}
	else {
		pDeviceContext->RSSetState(pWireRasterizerState);
	}

	UINT pStrides = sizeof(vertex3D);
	UINT pOffsets = 0;
	pDeviceContext->IASetPrimitiveTopology(/*D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP*/D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST/*D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	//pDeviceContext->PSSetShaderResources(0, 1, &subsetsList[0].diffuse.pShaderResourceView);
	pDeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	//pDeviceContext->DrawIndexed(indexCount, 0, 0);
	pDeviceContext->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &pStrides, &pOffsets);
	pDeviceContext->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	for (Subset subset : mesh.subsetsList) {
		pDeviceContext->PSSetShaderResources(0, 1, &subset.diffuse.pShaderResourceView);
		pDeviceContext->DrawIndexed(subset.startIndex + subset.IndexCount, 0, 0);
	}
}

void SkinnedMesh::setProjection(ID3D11DeviceContext *pDeviceContext, const Transform &preSetTransform, const Transform& transform, const XMMATRIX& globalTransform, SkeletalAnimation& skeletalAnimation, const int& animationFrame, float elapsedTime)
{
	static XMFLOAT3 position, rotationAxis;
	//position = toNDC(transform.position + preSetTransform.position);
	rotationAxis = toNDC(transform.rotationAxis);
	position = transform.position + preSetTransform.position;
	//rotationAxis = 
	static XMMATRIX S, R, T, W, V, P, WVP;
	S = R = T = W = V = P = WVP = DirectX::XMMatrixIdentity();
	S = DirectX::XMMatrixScaling(transform.scaling.x*preSetTransform.scaling.x, transform.scaling.y*preSetTransform.scaling.y, transform.scaling.z*preSetTransform.scaling.z);
	//R = DirectX::XMMatrixRotationAxis(XMVectorSet(rotationAxis.x, rotationAxis.y, rotationAxis.z, 0), transform2D3D->angle*0.01745329251);
	R =	DirectX::XMMatrixRotationX((transform.eulerAngle.x + preSetTransform.eulerAngle.x)*0.01745)
		*DirectX::XMMatrixRotationY((transform.eulerAngle.y + preSetTransform.eulerAngle.y)*0.01745)
		*DirectX::XMMatrixRotationZ((transform.eulerAngle.z + preSetTransform.eulerAngle.z)*0.01745);
	//R = DirectX::XMMatrixRotationRollPitchYaw((transform.eulerAngle.y)*0.01745, transform.eulerAngle.x*0.01745, transform.eulerAngle.z*0.01745);
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	W = S*R*T;
	
	V = DirectX::XMMatrixLookAtLH(e_mainCamera.eyePosition, e_mainCamera.focusPosition, e_mainCamera.upDirection);
	P = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f);
	/*W = DirectX::XMMatrixTranspose(W);
	V = DirectX::XMMatrixTranspose(V);
	P = DirectX::XMMatrixTranspose(P);*/
	WVP = W*V*P;

	//D3D11_MAPPED_SUBRESOURCE mappedSubRec;
	//hr = _DeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD/*D3D11_MAP_WRITE_NO_OVERWRITE*/, 0, &mappedSubRec);
	//if (FAILED(hr))
	//{
	//	MessageBox(0, L"Update pConstantBuffer failed", L"SkinnedMesh::render()", 0);
	//	return;
	//}

	static VS_CBUFFER_PROJECTION updateCbuffer;
	updateCbuffer.world = globalTransform*coordinateConversion*W;
	updateCbuffer.view = V;
	updateCbuffer.projection = P;
	updateCbuffer.worldViewProjection = globalTransform*coordinateConversion*WVP;
	static XMVECTOR lightDirection = { 0.0f,0.0f,1.0f,0.0f };
	lightDirection = e_mainCamera.focusPosition - e_mainCamera.eyePosition;
	updateCbuffer.lightDirection = XMFLOAT4(lightDirection.vector4_f32);
	updateCbuffer.materialColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//Play Fbx Animation
	if (skeletalAnimation.size() > 0) {
		if (animationFrame == 0)
		{
			frame = skeletalAnimation.animationTick / skeletalAnimation.samplingTime;
		}
		else
		{
			frame = animationFrame;
		}
		
		if (frame > skeletalAnimation.size() - 1) {
			frame = 0;
			skeletalAnimation.animationTick = 0;
		}
		Skeletal skeletal = skeletalAnimation.at(frame);
		size_t bonesNum = skeletal.size();
		_ASSERT_EXPR(bonesNum < MAX_BONES, L"The bonesNum exceeds MAX_BONES!");
		for (size_t i = 0; i < bonesNum; i++) {
			updateCbuffer.boneTransforms[i] = skeletal[i].transform;
		}
		skeletalAnimation.animationTick += elapsedTime;
	}

	pDeviceContext->UpdateSubresource(pConstantBuffer, 0, NULL, &updateCbuffer, 0, 0);

	//_DeviceContext->Unmap(pConstantBuffer, 0);
}


void SkinnedMesh::drawMesh(ID3D11DeviceContext *pDeviceContext, const Transform& preSetTransform, const Transform& transform, const int& animationFrame, float elapsedTime)
{
	for (Mesh &mesh : meshesList)
	{
		setProjection(pDeviceContext, preSetTransform, transform, mesh.globalTransform, mesh.skeletalAnimation, animationFrame, elapsedTime);
		render(pDeviceContext, true, mesh);
	}
}

SkinnedMesh::~SkinnedMesh()
{
	for (Mesh &mesh : meshesList)
	{
		SAFE_RELEASE(mesh.vertexBuffer);
		SAFE_RELEASE(mesh.indexBuffer);
		for (Subset &subset : mesh.subsetsList) {
			SAFE_RELEASE(subset.diffuse.pShaderResourceView);
		}
	}

	SAFE_RELEASE(pConstantBuffer);

	SAFE_RELEASE(pWireRasterizerState);
	SAFE_RELEASE(pFillRasterizerState);

	SAFE_RELEASE(pDepthStencilState);

	RM::releasePixelShader(pPixelShader);
	RM::releaseVertexShader(pVertexShader, pInputLayout);

	SAFE_RELEASE(pSamplerState);

}

XMFLOAT3 SkinnedMesh::toNDC(XMFLOAT3 input)
{
	float x, y, z;
	// 2D without projection
	/*x = 2.0f*_coord.x / SCREEN_WIDTH - 1.0f;
	y = 1.0f - 2.0f*_coord.y / SCREEN_WIDTH;*/

	// 2D with projection
	x = input.x / (float)SCREEN_WIDTH;
	y = input.y / (float)SCREEN_WIDTH;
	z = input.z / (float)SCREEN_WIDTH;
	return XMFLOAT3(x, y, z);
}