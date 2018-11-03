#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "3dgp_system.h"
#include "blend.h"

#include "sprite.h"

#include "primitive3d.h"
#include <ctime>

#include "render_target.h"

#pragma comment(lib, "winmm")

#define FPS (60)

class Scene;

class framework
{
private:
	static Scene* s_pScene;
	bool isFocused = true;

public:
	static void changeScene(Scene* pNextScene) {
		if (pNextScene) {
			s_pScene = pNextScene;
		}
	};

	const double MIN_FRAME_TIME_DAFAULT = 1.0 / (double)FPS;
	double minFrameTime = MIN_FRAME_TIME_DAFAULT;
	double frameTime = 0.0f;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;

	void setFPSLimitation(int limation = 20);
	bool isFullScreen;

public:
	HWND hwnd = NULL;
	/*static CONST LONG SCREEN_WIDTH = SCREEN_WIDTH;
	static CONST LONG SCREEN_HEIGHT = SCREEN_HEIGHT;*/

	HRESULT hr;

	UINT createDeviceFlags = 0;

	static ID3D11Device*			s_pDevice;
	static ID3D11DeviceContext*		s_pDeviceContext;

	ID3D11Debug* pD3dDebug;


	static ID3D11RenderTargetView*	s_pRenderTargetView;
	static ID3D11DepthStencilView*	s_pDepthStencilView;
	ID3D11DepthStencilState*		pDepthStencilState;

	D3D_DRIVER_TYPE					driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL				featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain*					pSwapChain = NULL;
	ID3D11Texture2D*				pDepthStencilResource = NULL;
	ID3D11BlendState*				pBlendState = NULL;

	Primitive3D *pPrimitive3D[20];

	int blendMode = 1;
	float alpha = 255.0f;
	const char strBlendMode[16][16] = {
		"NONE",
		"ALPHA",
		"ADD",
		"SUBTRACT",
		"REPLACE",
		"MULTIPLY",
		"LIGHTEN",
		"DARKEN",
		"SCREEN"
	};

	framework(HWND _hwnd) /*: hwnd(hwnd)*/
	{
		//MessageBox(0, L"Constructor called", L"framework", MB_OK);
		if (!initialize(_hwnd)) {
			MessageBox(0, L"run: Iniialize FAILED", 0, 0);
			return;
		}
	}
	~framework()
	{
		release();
	}
	int run();

	LRESULT CALLBACK handle_message(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	bool initialize(HWND _hwnd);
	void update(float elapsed_time = .0f/*Elapsed seconds from last frame*/);
	void render(float elapsed_time = .0f/*Elapsed seconds from last frame*/);
	void release();

private:
	high_resolution_timer timer;
	void calculate_frame_stats();
};


#endif // !_FRAMEWORK_H_