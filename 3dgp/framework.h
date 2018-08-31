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
	bool m_isFocused = true;

public:
	static void changeScene(Scene* a_pNextScene) {
		if (a_pNextScene) {
			s_pScene = a_pNextScene;
		}
	};

	const double MIN_FRAME_TIME_DAFAULT = 1.0 / (double)FPS;
	double m_minFrameTime = MIN_FRAME_TIME_DAFAULT;
	double m_frameTime = 0.0f;
	LARGE_INTEGER m_timeStart;
	LARGE_INTEGER m_timeEnd;
	LARGE_INTEGER m_timeFreq;

	void setFPSLimitation(int a_FPS = 20);
	bool m_isFullScreen;

public:
	HWND m_hWnd = NULL;
	/*static CONST LONG SCREEN_WIDTH = SCREEN_WIDTH;
	static CONST LONG SCREEN_HEIGHT = SCREEN_HEIGHT;*/

	HRESULT hr;

	UINT createDeviceFlags = 0;

	static ID3D11Device*			s_pDevice;
	static ID3D11DeviceContext*		s_pDeviceContext;

	static ID3D11RenderTargetView*	s_pRenderTargetView;
	static ID3D11DepthStencilView*	s_pDepthStencilView;
	ID3D11DepthStencilState*		m_pDepthStencilState;

	D3D_DRIVER_TYPE					m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL				m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain*					m_pSwapChain = NULL;
	ID3D11Texture2D*				m_pDepthStencilResource = NULL;
	ID3D11BlendState*				m_pBlendState = NULL;

	Primitive3D *m_pPrimitive3D[20];

	RenderTarget *m_pRenderTargets[20];

	int blendMode = 1;
	float m_alpha = 255.0f;
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

	framework(HWND hwnd) /*: hwnd(hwnd)*/
	{
		//MessageBox(0, L"Constructor called", L"framework", MB_OK);
		if (!initialize(hwnd)) {
			MessageBox(0, L"run: Iniialize FAILED", 0, 0);
			return;
		}
	}
	~framework()
	{
		release();
	}
	int run();

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	bool initialize(HWND hwnd);
	void update(float elapsed_time = .0f/*Elapsed seconds from last frame*/);
	void render(float elapsed_time = .0f/*Elapsed seconds from last frame*/);
	void release();

private:
	high_resolution_timer m_timer;
	void calculate_frame_stats();
};


#endif // !_FRAMEWORK_H_