#ifndef _SYSTEM_
#define _SYSTEM_

#include <d3d11.h>

#define _3DGP_SYS_

namespace GLC 
{
	class System
	{
	private:
		System() = default;
		~System() = default;
	public:
		static D3D_DRIVER_TYPE             driverType;
		static D3D_FEATURE_LEVEL           featureLevel;
		static ID3D11Device*               pd3dDevice;
		static ID3D11DeviceContext*        pImmediateContext;
		static IDXGISwapChain*             pSwapChain;
		static ID3D11RenderTargetView*     pRenderTargetView;
		static ID3D11Texture2D*            pDepthStencil;
		static ID3D11DepthStencilView*     pDepthStencilView;
		static ID3D11DepthStencilState*    pDepthStencilState;

		static UINT windowWidth;
		static UINT windowHeight;
		static HWND outputWindow;


		// Initialize D3D COM
		static HRESULT Initialize(HWND hWnd);

		// Clear the backbuffer with RGBA(0,0,0,1.0f)
		// Set default DepthStencilState
		static void Clear();

		static void Release();

		static void ReportLiveObjects();
	};

}


#endif // !_SYSTEM_
