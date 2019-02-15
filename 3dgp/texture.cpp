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
	strcpy_s(this->fileName, fileName);
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
		RM::LoadShaderResourceView(System::pd3dDevice, &shaderResourceViewRef, fileName);
	}
	else {
		RM::LoadShaderResourceView(System::pd3dDevice, &shaderResourceViewRef, nullptr);
	}
}

void Texture::Load(Texture textures[])
{
	for (int i = 0; textures[i].fileName != nullptr && i < RM::FILE_NUM_MAX; ++i) {
		textures[i].Load();
	}
}

ID3D11ShaderResourceView* Texture::GetData()
{
	return shaderResourceViewRef;
}
