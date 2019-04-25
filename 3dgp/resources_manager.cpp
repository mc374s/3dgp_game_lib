#include "resources_manager.h"

#include "WICTextureLoader.h"
#include <cstdio>
//#include <d3dcompiler.h>
using namespace RM;

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// 画像データの管理
int ResourcesManager::s_imgFileCounter = 0;
D3D11_RESOURCES<ID3D11ShaderResourceView*> ResourcesManager::s_SRVResources[FILE_NUM_MAX];

int RM::LoadShaderResourceView(ID3D11Device* pDevice, ID3D11ShaderResourceView** ppOutSRV, char* pFileName, ID3D11Resource** ppOutResource) {
	int fileNO = 0;

	// すでに存在しているリソースがあれば、レファレンス作成。無ければ新規作成
	for (fileNO = 0; fileNO < ResourcesManager::s_imgFileCounter; ++fileNO)
	{
		// pFileName == nullptrのときにダミーSRVを参照
		if ((pFileName && strcmp(ResourcesManager::s_SRVResources[fileNO].pFileName, pFileName) == 0)
			|| (!pFileName && strcmp(ResourcesManager::s_SRVResources[fileNO].pFileName, "null") == 0))
		{
			//　リソースを ppOutResource に返す
			if (ppOutResource) {
				ResourcesManager::s_SRVResources[fileNO].pData->GetResource(ppOutResource);
			}
			ResourcesManager::s_SRVResources[fileNO].fileRefNum++;
			break;
		}
	}

	//　新規リソース
	if (fileNO == ResourcesManager::s_imgFileCounter)
	{
		HRESULT hr;
		// pFileName が nullptr の場合はダミーSRVを作成
		if (pFileName)
		{
			const size_t cSize = strlen(pFileName) + 1;
			wchar_t *wcFileName = new wchar_t[cSize];
			mbstowcs_s(nullptr, wcFileName, cSize, pFileName, cSize);
			hr = DirectX::CreateWICTextureFromFile(pDevice, wcFileName, ppOutResource, &ResourcesManager::s_SRVResources[fileNO].pData);
			delete[] wcFileName;
			if (FAILED(hr))
			{
				MessageBox(0, L"CreateWICTextureFromFile Filed!", L"MyResourceManager::loadResource()", MB_OK);
				return -1;
			}
			strcpy_s(ResourcesManager::s_SRVResources[fileNO].pFileName, pFileName);
		}
		else
		{
			hr = ResourcesManager::MakeDummyShaderResourceView(pDevice, &ResourcesManager::s_SRVResources[fileNO].pData);
			if (FAILED(hr))
			{
				MessageBox(0, L"MakeDummyShaderResourceView Filed!", L"MyResourceManager::loadResource()", MB_OK);
				return -1;
			}
			strcpy_s(ResourcesManager::s_SRVResources[fileNO].pFileName, "null");
		}

		ResourcesManager::s_SRVResources[fileNO].fileRefNum++;
		ResourcesManager::s_imgFileCounter++;

	}
	// SRV出力
	*ppOutSRV = ResourcesManager::s_SRVResources[fileNO].pData;

	//　リソースの番号を返す
	return fileNO;
}

void RM::ReleaseShaderResourceView(ID3D11ShaderResourceView* pInSRV) {
	if (pInSRV)
	{
		for (int i = 0; i < ResourcesManager::s_imgFileCounter; ++i)
		{
			if (ResourcesManager::s_SRVResources[i].pData == pInSRV)
			{
				ResourcesManager::s_SRVResources[i].Release();
				pInSRV = nullptr;
				break;
			}
		}
	}
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// バーティクスシェーダーソースの管理
int ResourcesManager::s_vsFileCounter = 0;
D3D11_RESOURCES<ID3D11VertexShader*> ResourcesManager::s_vertexShaderResources[FILE_NUM_MAX];
ID3D11InputLayout* ResourcesManager::s_inputLayoutResources[FILE_NUM_MAX];

int RM::LoadVertexShader(ID3D11Device* pDevice, char* pFilename,const D3D11_INPUT_ELEMENT_DESC* pInLayoutDesc, int elementsNum, ID3D11VertexShader** ppOutVertexShader, ID3D11InputLayout** ppOutInputLayout)
{
	int fileNO = 0;

	// すでに存在しているリソース
	for (fileNO = 0; fileNO < ResourcesManager::s_vsFileCounter; ++fileNO)
	{
		if (strcmp(ResourcesManager::s_vertexShaderResources[fileNO].pFileName, pFilename) == 0)
		{
			ResourcesManager::s_vertexShaderResources[fileNO].fileRefNum++;
			break;
		}
	}

	//　新規リソース
	if (fileNO == ResourcesManager::s_vsFileCounter)
	{
		unsigned char *cso_data = NULL;
		UINT cso_sz;

		FILE *fp = NULL;
		if (fopen_s(&fp, pFilename, "rb") != 0)
			MessageBox(0, L"resources_manager: open vs file failed", 0, 0);
		fseek(fp, 0, SEEK_END);
		cso_sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		cso_data = new unsigned char[cso_sz];
		fread(cso_data, cso_sz, 1, fp);
		fclose(fp);
		// Create the vertex shader
		HRESULT hr = pDevice->CreateVertexShader(cso_data, cso_sz, NULL, &ResourcesManager::s_vertexShaderResources[fileNO].pData);
		//pDevice->CreateVertexShader()
		if (FAILED(hr))
		{
			MessageBox(0, L"CreateVertexShader failed", L"ResourceManager::LoadVertexShader()", MB_OK);
			return -1;
		}

		// Create the input layout
		hr = pDevice->CreateInputLayout(pInLayoutDesc, elementsNum, cso_data, cso_sz, &ResourcesManager::s_inputLayoutResources[fileNO]);
		if (FAILED(hr))
		{
			MessageBox(0, L"CreateInputLayout failed", L"ResourceManager::LoadVertexShader()", MB_OK);
			return -1;
		}
		delete[] cso_data;

		//ResourcesManager::s_vertexShaderResources[fileNO].pFileName = pFileName;
		strcpy_s(ResourcesManager::s_vertexShaderResources[fileNO].pFileName, pFilename);
		ResourcesManager::s_vertexShaderResources[fileNO].fileRefNum++;
		ResourcesManager::s_vsFileCounter++;
	}

	// VertexShaderとInputLayoutの出力
	*ppOutVertexShader = ResourcesManager::s_vertexShaderResources[fileNO].pData;
	*ppOutInputLayout = ResourcesManager::s_inputLayoutResources[fileNO];

	//　リソースの番号を返す
	return fileNO;
};


void RM::ReleaseVertexShader(ID3D11VertexShader* pInVertexShader, ID3D11InputLayout* pInInputLayout) {
	if (pInVertexShader)
	{
		for (int i = 0; i < ResourcesManager::s_vsFileCounter; ++i)
		{
			if (ResourcesManager::s_vertexShaderResources[i].pData == pInVertexShader)
			{
				ResourcesManager::s_vertexShaderResources[i].Release();
				if (ResourcesManager::s_vertexShaderResources[i].fileRefNum <= 0)
				{
					SAFE_RELEASE(ResourcesManager::s_inputLayoutResources[i]);
				}
				pInVertexShader = nullptr;
				pInInputLayout = nullptr;
				break;
			}
		}
	}
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// ピクセルシェーダーソースの管理
int ResourcesManager::s_psFileCounter = 0;
D3D11_RESOURCES<ID3D11PixelShader*> ResourcesManager::s_pixelShaderResources[FILE_NUM_MAX];

int RM::LoadPixelShader(ID3D11Device* pDevice, char* pFilename, ID3D11PixelShader** ppOut)
{
	int fileNO = 0;

	// すでに存在しているリソース
	for (fileNO = 0; fileNO < ResourcesManager::s_psFileCounter; ++fileNO)
	{
		if (strcmp(ResourcesManager::s_pixelShaderResources[fileNO].pFileName, pFilename) == 0)
		{
			ResourcesManager::s_pixelShaderResources[fileNO].fileRefNum++;
			break;
		}
	}

	//　新規リソース
	if (fileNO == ResourcesManager::s_psFileCounter)
	{
		unsigned char *cso_data = NULL;
		UINT cso_sz;

		FILE *fp = NULL;
		if (fopen_s(&fp, pFilename, "rb") != 0)
			MessageBox(0, L"resource_manager: open ps file failed", 0, 0);
		fseek(fp, 0, SEEK_END);
		cso_sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		cso_data = new unsigned char[cso_sz];
		fread(cso_data, cso_sz, 1, fp);
		fclose(fp);
		// Create the pixel shader
		HRESULT hr = pDevice->CreatePixelShader(cso_data, cso_sz, NULL, &ResourcesManager::s_pixelShaderResources[fileNO].pData);
		if (FAILED(hr))
		{
			MessageBox(0, L"sprite: CreatePixelShader failed", 0, 0);
			return false;
		}
		delete[] cso_data;

		// リソースカウンタアップ
		strcpy_s(ResourcesManager::s_pixelShaderResources[fileNO].pFileName, pFilename);
		//ResourcesManager::s_pixelShaderResources[fileNO].pFileName = pFileName;
		ResourcesManager::s_pixelShaderResources[fileNO].fileRefNum++;
		ResourcesManager::s_psFileCounter++;
	}
	// PixelShader Data出力
	*ppOut = ResourcesManager::s_pixelShaderResources[fileNO].pData;
	//　リソースの番号を返す
	return fileNO;

}

void RM::ReleasePixelShader(ID3D11PixelShader* pIn) {
	if (pIn)
	{
		for (int i = 0; i < ResourcesManager::s_psFileCounter; ++i)
		{
			if (ResourcesManager::s_pixelShaderResources[i].pData == pIn)
			{
				ResourcesManager::s_pixelShaderResources[i].Release();
				pIn = nullptr;
				break;
			}
		}
	}
}

HRESULT	ResourcesManager::MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texture2d_desc = {};
	texture2d_desc.Width = 1;
	texture2d_desc.Height = 1;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	u_int color = 0xFFFFFFFF;
	subresource_data.pSysMem = &color;
	subresource_data.SysMemPitch = 4;
	subresource_data.SysMemSlicePitch = 4;

	ID3D11Texture2D *texture2d;
	hr = Device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
	if (hr)		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;

	hr = Device->CreateShaderResourceView(texture2d, &shader_resource_view_desc, shaderResourceView);
	if (hr)		return hr;

	texture2d->Release();

	return hr;
}