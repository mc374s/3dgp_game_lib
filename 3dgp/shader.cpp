#include "shader.h"

#include "resources_manager.h"
#include "system.h"

using namespace GLC;

Shader::Shader()
{
	vertexShaderRef = nullptr;
	pixelShaderRef = nullptr;
	inputLayoutRef = nullptr;
	inputElementDescs = nullptr;
	inputElementsNum = 0;
}

Shader::Shader(const char* vertexCSOfileName, const char* pixelCSOfileName, const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, const int elementsNum)
{
	Shader();

	// Copy .cso files' name.
	strcpy_s(this->vertexCSOfileName, vertexCSOfileName);
	strcpy_s(this->pixelCSOfileName, pixelCSOfileName);

	// Create inputElementDescs, attention to the memory leak.
	this->inputElementDescs = new D3D11_INPUT_ELEMENT_DESC[elementsNum];
	memcpy_s(this->inputElementDescs, sizeof(D3D11_INPUT_ELEMENT_DESC)*elementsNum, inputElementDescs, sizeof(D3D11_INPUT_ELEMENT_DESC)*elementsNum);
	this->inputElementsNum = elementsNum;

}

Shader::~Shader()
{
	// Release resource reference.
	RM::ReleasePixelShader(pixelShaderRef);
	RM::ReleaseVertexShader(vertexShaderRef, inputLayoutRef);
	SAFE_DELETE(inputElementDescs);
}

void Shader::Load()
{
	// Create Shader and InputLayout by RespurceManager.
	RM::LoadVertexShader(System::pd3dDevice, vertexCSOfileName, inputElementDescs, inputElementsNum, &vertexShaderRef, &inputLayoutRef);
	RM::LoadPixelShader(System::pd3dDevice, pixelCSOfileName, &pixelShaderRef);
	//SAFE_DELETE(inputElementDescs);
}

void Shader::Set()
{
	System::pImmediateContext->IASetInputLayout(inputLayoutRef);
	System::pImmediateContext->VSSetShader(vertexShaderRef, nullptr, 0);
	System::pImmediateContext->PSSetShader(pixelShaderRef, nullptr, 0);
}