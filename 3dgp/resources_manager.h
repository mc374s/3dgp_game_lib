#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <d3d11.h>
#include "WICTextureLoader.h"

//Namespace ResourcesManager
namespace RM {

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

		// �摜�f�[�^�̊Ǘ�
		static int s_imgFileCounter;
		static D3D11_RESOURCES<ID3D11ShaderResourceView*> s_SRVResources[FILE_NUM_MAX];

		// �o�[�e�B�N�X�V�F�[�_�[�f�[�^�̊Ǘ�
		static int s_vsFileCounter;
		static D3D11_RESOURCES<ID3D11VertexShader*> s_vertexShaderResources[FILE_NUM_MAX];
		static ID3D11InputLayout* s_inputLayoutResources[FILE_NUM_MAX];

		// �s�N�Z���V�F�[�_�[�f�[�^�̊Ǘ�
		static int s_psFileCounter;
		static D3D11_RESOURCES<ID3D11PixelShader*> s_pixelShaderResources[FILE_NUM_MAX];

	public:
		// �摜�f�[�^�̊Ǘ�
		friend int loadShaderResourceView(ID3D11Device* pDevice, char* pFilename, ID3D11Resource** ppOutResource, ID3D11ShaderResourceView** ppOutSRV);
		friend void releaseShaderResourceView(ID3D11ShaderResourceView* pInSRV);

		// �o�[�e�B�N�X�V�F�[�_�[�\�[�X�̊Ǘ�
		friend int loadVertexShader(ID3D11Device* pDevice, char* pFilename, D3D11_INPUT_ELEMENT_DESC* pInLayoutElements, int elementsNum, ID3D11VertexShader** ppOutVertexShader, ID3D11InputLayout** ppOutInputLayout);

		friend void releaseVertexShader(ID3D11VertexShader* pInVertexShader, ID3D11InputLayout* pInInputLayout);

		// �s�N�Z���V�F�[�_�[�f�[�^�̊Ǘ�
		friend int loadPixelShader(ID3D11Device* pDevice, char* pFilename, ID3D11PixelShader** ppOut);
		friend void releasePixelShader(ID3D11PixelShader* pIn);
	};// class ResourcesManager
	/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

	HRESULT	MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView);

}//namespace RM

//namespace RM = RM;

#endif // !_RESOURCES_MANAGER_H_
