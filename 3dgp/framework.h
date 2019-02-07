#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#pragma comment(lib, "winmm")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "DirectXTK")

#include <d3d11.h>


#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define FPS (60)

class HighResolutionTimer;

class Scene;
class Primitive3D;


class Framework
{
private:
	static Scene* pScene;
	bool isFocused = true;

public:
	static void changeScene(Scene* pNextScene) {
		if (pNextScene) {
			pScene = pNextScene;
		}
	};

	const double MIN_FRAME_TIME_DAFAULT = 1.0 / (double)FPS;
	double minFrameTime = MIN_FRAME_TIME_DAFAULT;
	static double frameTime;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;

	void setFPSLimitation(int limitation = FPS);
	bool isFullScreen;

public:
	HWND outputWindow = NULL;

	HRESULT hr;

	UINT createDeviceFlags = 0;

	static ID3D11Device*			pDevice;
	static ID3D11DeviceContext*		pDeviceContext;


	static ID3D11RenderTargetView*	pRenderTargetView;
	static ID3D11DepthStencilView*	pDepthStencilView;
	ID3D11DepthStencilState*		pDepthStencilState;

	D3D_DRIVER_TYPE					driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL				featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain*					pSwapChain = NULL;
	ID3D11Texture2D*				pDepthStencilResource = NULL;
	ID3D11BlendState*				pBlendState = NULL;


	Framework(HWND _hwnd) /*: hwnd(hwnd)*/
	{
		//MessageBox(0, L"Constructor called", L"framework", MB_OK);
		if (!Initialize(_hwnd)) {
			MessageBox(0, L"Run: Iniialize FAILED", 0, 0);
			return;
		}
	}
	~Framework()
	{
		Release();
	}
	int Run();

	LRESULT CALLBACK HandleMessage(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	bool Initialize(HWND _hwnd);
	void Update(float elapsed_time = .0f/*Elapsed seconds from last frame*/);
	void Draw(float elapsed_time = .0f/*Elapsed seconds from last frame*/);
	void Release();

private:
	HighResolutionTimer* timer;
	void CalculateFrameStats();
};


#endif // !_FRAMEWORK_H_