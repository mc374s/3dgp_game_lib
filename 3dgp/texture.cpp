#include "texture.h"
#include "resources_manager.h"

#include "system.h"

using namespace GLC;

Texture::Texture()
{
	strcpy_s(fileName, "null");
	shaderResourceViewRef = nullptr;
}

Texture::Texture(const char* fileName)
{
	if (fileName) {
		strcpy_s(this->fileName, fileName);
	}
	else {
		strcpy_s(this->fileName, "null");
	}
	shaderResourceViewRef = nullptr;
}

Texture::~Texture()
{
	// Release resource reference
	RM::ReleaseShaderResourceView(shaderResourceViewRef);
}

void Texture::Load()
{
	// Load ShaderResourceView from textures' @fileName
	// If @fileName == "null", Load a dammy SRV
	if (fileName && strcmp(fileName, "null") != 0) {
		ID3D11Resource* resource = nullptr;
		RM::LoadShaderResourceView(System::pd3dDevice, &shaderResourceViewRef, fileName, &resource);
		// If load successed, get image's width and height
		if (resource) {
			ID3D11Texture2D* texture2d = nullptr;
			resource->QueryInterface(&texture2d);
			if (texture2d) {
				D3D11_TEXTURE2D_DESC texture2dDesc;
				texture2d->GetDesc(&texture2dDesc);
				width = texture2dDesc.Width;
				height = texture2dDesc.Height;
				texture2d->Release();
			}
			resource->Release();
		}
	}
	else {
		RM::LoadShaderResourceView(System::pd3dDevice, &shaderResourceViewRef, nullptr);
	}
}

void Texture::Load(Texture textures[])
{
	for (int i = 0; strcmp(textures[i].fileName, "null") != 0 && i < RM::FILE_NUM_MAX; ++i) {
		textures[i].Load();
	}
}

ID3D11ShaderResourceView* Texture::GetData()
{
	return shaderResourceViewRef;
}
