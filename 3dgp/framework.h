#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "3dgp_system.h"
#include <ctime>

#include "sprite.h"
#include "primitive3d.h"
#include "skinned_mesh.h"


#define FPS (60)

#define N	(10)

#pragma comment(lib, "winmm")



class framework
{
public:
	const float MIN_FRAME_TIME_DAFAULT = 1.0f / FPS;
	float m_minFrameTime = MIN_FRAME_TIME_DAFAULT;
	float m_frameTime = 0;
	LARGE_INTEGER m_timeStart;
	LARGE_INTEGER m_timeEnd;
	LARGE_INTEGER m_timeFreq;

	void setFPSLimitation(int a_FPS = 20);

public:
	HWND m_hWnd = NULL;
	/*static CONST LONG SCREEN_WIDTH = SCREEN_WIDTH;
	static CONST LONG SCREEN_HEIGHT = SCREEN_HEIGHT;*/

	HRESULT hr;

	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pDevice = NULL;
	ID3D11DeviceContext*    m_pDeviceContext = NULL;
	IDXGISwapChain*         m_pSwapChain = NULL;
	ID3D11Texture2D*        m_pDepthStencilResource = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11BlendState*		m_pBlendState = NULL;

	IMFPMediaPlayer*		m_pMFPlayer = NULL;

	Sprite* m_pSprites[1024];


	//MyGeometricPrimitive *m_pPrimitive3D;
	Primitive3D *m_pPrimitive3D[16];
	SkinnedMesh *m_pMesh[16];


	LARGE_INTEGER frame[N] = { 0 };
	LARGE_INTEGER max = { 0 };
	int frameCounter = 0;

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
	bool keyDown = false;


	framework(HWND hwnd);
	~framework();
	int run();

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	bool initialize(HWND hwnd);
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	void release();

private:
	high_resolution_timer timer;
	void calculate_frame_stats();
};


#endif // !_FRAMEWORK_H_