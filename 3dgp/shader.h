#ifndef _SHADER_H_
#define _SHADER_H_

#include <d3d11.h>

namespace GLC 
{
	class Shader
	{
	private:
		ID3D11VertexShader* vertexShaderRef;
		ID3D11PixelShader*  pixelShaderRef;
		ID3D11InputLayout*  inputLayoutRef;

		D3D11_INPUT_ELEMENT_DESC* inputElementDescs;
		int inputElementsNum;

	public:
		Shader();
		~Shader();

		char vertexCSOfileName[256];
		char pixelCSOfileName[256];

		// Set .cso file name and imput elements.
		Shader(const char* vertexCSOfileName, const char* pixelCSOfileName, const D3D11_INPUT_ELEMENT_DESC* inputElementDescs,const int elementsNum);

		// Create Shader from .cso file.
		void Load();

		// Set input layout, vertex shader, pixel shader here.
		void Set();

	};

}; // namespace GLC



#endif // !_SHADER_H_

