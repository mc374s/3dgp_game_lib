#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <d3d11.h>
//
//Namespace ResourcesManager
//
namespace RM {

	#define SAFE_RELEASE(ptr) {if(ptr){ptr->Release(); ptr=nullptr;}}
	#define SAFE_DELETE(ptr) {if(ptr){delete ptr; ptr=nullptr;}}

	template <typename D3D_TYPE>
	struct D3D11_RESOURCES {
		D3D_TYPE pData;
		char pFileName[256];
		int fileRefNum = 0;
		D3D11_RESOURCES() :pData(NULL), fileRefNum(0) {};
		void Release() {
			fileRefNum--;
			if (fileRefNum <= 0) {
				SAFE_RELEASE(pData);
				ZeroMemory(pFileName, sizeof(pFileName));
				fileRefNum = 0;
			}
		};
	};

	/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	static const int FILE_NUM_MAX = 64;
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

		// ダミーテクスチャを作成する
		static HRESULT	MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView);

	public:
		// 画像データの管理

		////重複の作成を回避するため、作成と伴にファレンスを作成する
		////pFileName が nullptr の場合ダミーテクスチャが作成される
		friend int LoadShaderResourceView(ID3D11Device* pDevice, ID3D11ShaderResourceView** ppOutSRV, char* pFilename = nullptr, ID3D11Resource** ppOutResource = nullptr);

		////リファレンスがすべてクリアしたらリソースを解放する
		friend void ReleaseShaderResourceView(ID3D11ShaderResourceView* pInSRV);

		// バーティクスシェーダーソースの管理

		////重複の作成を回避するため、作成と伴にファレンスを作成する
		friend int LoadVertexShader(ID3D11Device* pDevice, char* pFilename, D3D11_INPUT_ELEMENT_DESC* pInLayoutElements, int elementsNum, ID3D11VertexShader** ppOutVertexShader, ID3D11InputLayout** ppOutInputLayout);
		
		////リファレンスがすべてクリアしたらリソースを解放する
		friend void ReleaseVertexShader(ID3D11VertexShader* pInVertexShader, ID3D11InputLayout* pInInputLayout);

		// ピクセルシェーダーデータの管理

		////重複の作成を回避するため、作成と伴にファレンスを作成する
		friend int LoadPixelShader(ID3D11Device* pDevice, char* pFilename, ID3D11PixelShader** ppOut);
		
		////リファレンスがすべてクリアしたらリソースを解放する
		friend void ReleasePixelShader(ID3D11PixelShader* pIn);
	};// class ResourcesManager
	/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

}//namespace RM


#endif // !_RESOURCES_MANAGER_H_
