#include "resources_manager.h"
#include <cstdio>

using namespace RM;

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// �摜�f�[�^�̊Ǘ�
int ResourcesManager::s_imgFileCounter = 0;
D3D11_RESOURCES<ID3D11ShaderResourceView*> ResourcesManager::s_SRVResources[FILE_NUM_MAX];

int RM::loadShaderResourceView(ID3D11Device* pDevice, char* pFilename, ID3D11Resource** ppOutResource, ID3D11ShaderResourceView** ppOutSRV) {
	int fileNO = 0;

	// ���łɑ��݂��Ă��郊�\�[�X
	for (fileNO = 0; fileNO < ResourcesManager::s_imgFileCounter; fileNO++)
	{
		if (ResourcesManager::s_SRVResources[fileNO].pFileName == pFilename)
		{
			//�@���\�[�X�� _resource �ɕԂ�
			ResourcesManager::s_SRVResources[fileNO].pData->GetResource(ppOutResource);
			ResourcesManager::s_SRVResources[fileNO].fileRefNum++;
			break;
		}
	}

	//�@�V�K���\�[�X
	if (fileNO == ResourcesManager::s_imgFileCounter)
	{
		const size_t cSize = strlen(pFilename) + 1;
		wchar_t *wcFileName = new wchar_t[cSize];
		size_t temp;
		mbstowcs_s(&temp, wcFileName, cSize, pFilename, cSize);
		HRESULT hr = DirectX::CreateWICTextureFromFile(pDevice, wcFileName, ppOutResource, &ResourcesManager::s_SRVResources[fileNO].pData);
		delete[] wcFileName;
		if (FAILED(hr))
		{
			MessageBox(0, L"CreateWICTextureFromFile Filed!", L"MyResourceManager::loadResource()", MB_OK);
			return -1;
		}
		ResourcesManager::s_SRVResources[fileNO].pFileName = pFilename;
		ResourcesManager::s_SRVResources[fileNO].fileRefNum++;

		ResourcesManager::s_imgFileCounter++;

	}
	// SRV�o��
	*ppOutSRV = ResourcesManager::s_SRVResources[fileNO].pData;

	//�@���\�[�X�̔ԍ���Ԃ�
	return fileNO;
}
// ���\�[�X�����
void RM::releaseShaderResourceView(ID3D11ShaderResourceView* pInSRV) {
	if (pInSRV)
	{
		for (int i = 0; i < ResourcesManager::s_imgFileCounter; i++)
		{
			if (ResourcesManager::s_SRVResources[i].pData == pInSRV)
			{
				ResourcesManager::s_SRVResources[i].release();
				break;
			}
		}
	}
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// �o�[�e�B�N�X�V�F�[�_�[�\�[�X�̊Ǘ�
int ResourcesManager::s_vsFileCounter = 0;
D3D11_RESOURCES<ID3D11VertexShader*> ResourcesManager::s_vertexShaderResources[FILE_NUM_MAX];
ID3D11InputLayout* ResourcesManager::s_inputLayoutResources[FILE_NUM_MAX];

int RM::loadVertexShader(ID3D11Device* pDevice, char* pFilename, D3D11_INPUT_ELEMENT_DESC* pInLayoutDesc, int elementsNum, ID3D11VertexShader** ppOutVertexShader, ID3D11InputLayout** ppOutInputLayout)
{
	int fileNo = 0;

	// ���łɑ��݂��Ă��郊�\�[�X
	for (fileNo = 0; fileNo < ResourcesManager::s_vsFileCounter; fileNo++)
	{
		if (ResourcesManager::s_vertexShaderResources[fileNo].pFileName == pFilename)
		{
			ResourcesManager::s_vertexShaderResources[fileNo].fileRefNum++;
			break;
		}
	}

	//�@�V�K���\�[�X
	if (fileNo == ResourcesManager::s_vsFileCounter)
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
		HRESULT hr = pDevice->CreateVertexShader(cso_data, cso_sz, NULL, &ResourcesManager::s_vertexShaderResources[fileNo].pData);
		if (FAILED(hr))
		{
			MessageBox(0, L"CreateVertexShader failed", L"ResourceManager::loadVertexShader()", MB_OK);
			return -1;
		}

		// Create the input layout
		hr = pDevice->CreateInputLayout(pInLayoutDesc, elementsNum, cso_data, cso_sz, &ResourcesManager::s_inputLayoutResources[fileNo]);
		if (FAILED(hr))
		{
			MessageBox(0, L"CreateInputLayout failed", L"ResourceManager::loadVertexShader()", MB_OK);
			return -1;
		}
		delete[] cso_data;

		ResourcesManager::s_vertexShaderResources[fileNo].pFileName = pFilename;
		ResourcesManager::s_vertexShaderResources[fileNo].fileRefNum++;
		ResourcesManager::s_vsFileCounter++;
	}

	// VertexShader��InputLayout�̏o��
	*ppOutVertexShader = ResourcesManager::s_vertexShaderResources[fileNo].pData;
	*ppOutInputLayout = ResourcesManager::s_inputLayoutResources[fileNo];

	//�@���\�[�X�̔ԍ���Ԃ�
	return fileNo;
};


void RM::releaseVertexShader(ID3D11VertexShader* pInVertexShader, ID3D11InputLayout* pInInputLayout) {
	if (pInVertexShader)
	{
		for (int i = 0; i < ResourcesManager::s_vsFileCounter; i++)
		{
			if (ResourcesManager::s_vertexShaderResources[i].pData == pInVertexShader)
			{
				ResourcesManager::s_vertexShaderResources[i].release();
				if (ResourcesManager::s_vertexShaderResources[i].fileRefNum <= 0)
				{
					SAFE_RELEASE(ResourcesManager::s_inputLayoutResources[i]);
				}
				break;
			}
		}
	}
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// �s�N�Z���V�F�[�_�[�\�[�X�̊Ǘ�
int ResourcesManager::s_psFileCounter = 0;
D3D11_RESOURCES<ID3D11PixelShader*> ResourcesManager::s_pixelShaderResources[FILE_NUM_MAX];

int RM::loadPixelShader(ID3D11Device* pDevice, char* pFilename, ID3D11PixelShader** ppOut)
{
	int fileNo = 0;

	// ���łɑ��݂��Ă��郊�\�[�X
	for (fileNo = 0; fileNo < ResourcesManager::s_psFileCounter; fileNo++)
	{
		if (ResourcesManager::s_pixelShaderResources[fileNo].pFileName == pFilename)
		{
			ResourcesManager::s_pixelShaderResources[fileNo].fileRefNum++;
			break;
		}
	}

	//�@�V�K���\�[�X
	if (fileNo == ResourcesManager::s_psFileCounter)
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
		HRESULT hr = pDevice->CreatePixelShader(cso_data, cso_sz, NULL, &ResourcesManager::s_pixelShaderResources[fileNo].pData);
		if (FAILED(hr))
		{
			MessageBox(0, L"sprite: CreatePixelShader failed", 0, 0);
			return false;
		}
		delete[] cso_data;

		// ���\�[�X�J�E���^�A�b�v
		ResourcesManager::s_pixelShaderResources[fileNo].pFileName = pFilename;
		ResourcesManager::s_pixelShaderResources[fileNo].fileRefNum++;
		ResourcesManager::s_psFileCounter++;
	}
	// PixelShader Data�o��
	*ppOut = ResourcesManager::s_pixelShaderResources[fileNo].pData;
	//�@���\�[�X�̔ԍ���Ԃ�
	return fileNo;

}

void RM::releasePixelShader(ID3D11PixelShader* pIn) {
	if (pIn)
	{
		for (int i = 0; i < ResourcesManager::s_psFileCounter; i++)
		{
			if (ResourcesManager::s_pixelShaderResources[i].pData == pIn)
			{
				ResourcesManager::s_pixelShaderResources[i].release();
				break;
			}
		}
	}
}

HRESULT	RM::MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView)
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