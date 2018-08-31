#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <d3d11.h>
#include "WICTextureLoader.h"

namespace MyResourcesManager {

	#define FILE_NUM_MAX (64)
	#define SAFE_RELEASE(ptr) {if(ptr){ptr->Release();}}
	#define SAFE_DELETE(ptr) {if(ptr){delete ptr; ptr=nullptr;}}

	template <typename D3D_TYPE>
	struct D3D11_RESOURCES {
		D3D_TYPE pData;
		char* pFileName = NULL;
		int fileRefNum = 0;
		D3D11_RESOURCES() :pData(NULL), pFileName(NULL), fileRefNum(0) {};
		void release() {
			fileRefNum--;
			if (fileRefNum <= 0) {
				SAFE_RELEASE(pData);
				pFileName = NULL;
				fileRefNum = 0;
			}
		};
	};

	/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	class  ResourcesManager
	{
	private:
		ResourcesManager() {};
		~ResourcesManager() {};

		// 画像データの管理
		static int s_imgFileCounter;
		static D3D11_RESOURCES<ID3D11ShaderResourceView*> s_SRVResources[FILE_NUM_MAX];

		// バーティクスシェーダーデータの管理
		static int s_vsFileCounter;
		static D3D11_RESOURCES<ID3D11VertexShader*> s_vertexShaderResources[FILE_NUM_MAX];
		static ID3D11InputLayout* s_inputLayoutResources[FILE_NUM_MAX];

		// ピクセルシェーダーデータの管理
		static int s_psFileCounter;
		static D3D11_RESOURCES<ID3D11PixelShader*> s_pixelShaderResources[FILE_NUM_MAX];

	public:
		// 画像データの管理
		friend int loadShaderResourceView(ID3D11Device* a_pDevice, char* a_pFilename, ID3D11Resource** a_ppOutResource, ID3D11ShaderResourceView** a_ppOutSRV);
		friend void releaseShaderResourceView(ID3D11ShaderResourceView* a_pInSRV);

		// バーティクスシェーダーソースの管理
		friend int loadVertexShader(ID3D11Device* a_pDevice, char* a_pFilename, D3D11_INPUT_ELEMENT_DESC* a_pInLayoutElements, int a_elementsNum, ID3D11VertexShader** a_ppOutVertexShader, ID3D11InputLayout** a_ppOutInputLayout);

		friend void releaseVertexShader(ID3D11VertexShader* a_pInVertexShader, ID3D11InputLayout* a_pInInputLayout);

		// ピクセルシェーダーデータの管理
		friend int loadPixelShader(ID3D11Device* a_pDevice, char* a_pFilename, ID3D11PixelShader** a_ppOut);
		friend void releasePixelShader(ID3D11PixelShader* a_pIn);
	};// class ResourcesManager
	/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	HRESULT	MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView);

}//namespace MyResourcesManager

//namespace MyResourcesManager = RM;

#endif // !_RESOURCES_MANAGER_H_
