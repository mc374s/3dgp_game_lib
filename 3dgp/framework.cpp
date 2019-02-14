#include "framework.h"
#include <ctime>
#include <cstdlib>

#include "resources_manager.h"

#include "high_resolution_timer.h"

#include "camera.h"
#include "sprite_string.h"
#include "primitive3d.h"
#include "mf_audio.h"
#include "scene.h"

#include "input.h"
#include "directxtk.h"

#include "system.h"
#include "state_subset.h"

#include "../Game/sound_data.h"
#include "../Game/sprite_data.h"
#include "../Game/scene_title.h"

using namespace GLC;
using namespace DirectX;


Scene*	Framework::pScene = nullptr;
double	Framework::frameTime = 0.0f;

Framework::Framework(HWND hWnd)
{
	if (!Initialize(hWnd)) {
		_RPT0(_CRT_ERROR, "Framework Iniialization Failed");
		return;
	}
}
Framework::~Framework()
{
	Release();
}

bool Framework::Initialize(HWND hWnd)
{
	// System stuffs Initialization

	// Frame timer
	timer = new HighResolutionTimer();

	// Initialize DirectX COM objects 
	if (FAILED(System::Initialize(hWnd))) {
		_RPT0(_CRT_ERROR, "System Initialization Failed");
		return false;
	}

	// Create DirectXTK objects
	if (FAILED(DXTK::Initialize(System::pd3dDevice, System::pImmediateContext))) {
		_RPT0(_CRT_ERROR, "DirectXTK Objects Initialization Failed");
		return false;
	}

	// State subsets initialization
	Blend::Initialize(System::pd3dDevice);
	Sampler::Initialize(System::pd3dDevice);
	
	// Ascii sprite font initialization
	SpriteString::Initialize(System::pd3dDevice);


	// Game stuffs Initialization

	std::srand(unsigned int(time(NULL)));

	// Load audio resources
	MFAudioManager::GetInstance()->LoadAudios(audio_data);
	
	// Load 2d image resources
	pTextureManager->LoadTextures(e_loadTexture);


	// Initialize the world matrices

	// Initialize the view matrix

	// Initialize the projection matrix

	// Set Entry Scene
	ChangeScene(SCENE_TITLE);

	return true;
}

void Framework::Release()
{
	Sampler::Release();
	Blend::Release();

	SpriteString::Release();
	DXTK::Release();
	System::Release();
	delete timer;

}

// Set limitation to 0 fps means no limitation
void Framework::SetFPSLimitation(int limitation)
{
	if (limitation <= 0) {
		minFrameTime = 0;
	}
	else {
		minFrameTime = 1.0 / (double)limitation;
	}
}

void Framework::ChangeScene(Scene* pNextScene) {
	if (pNextScene) {
		pScene = pNextScene;
	}
}

int Framework::Run()
{
	MSG msg = {};

	srand((unsigned int)time(NULL));

	// Reset Input device
	for (int padIndex = 0; padIndex < Input::MAX_PLAYER_COUNT; ++padIndex) {
		Input::PAD_TRACKER[padIndex].Reset();
	}
	Input::KEY_TRACKER.Reset();


	// Check current device whether it was supported QueryPerformance function
	if (QueryPerformanceFrequency(&timeFreq) == false)
	{
		MessageBox(0, L"This Device is too old, QueryPerformanceFrequency failed", L"Framework", MB_OK);
		exit(-1);
	}
	QueryPerformanceCounter(&timeStart);


	DWORD sleepTime;

	// Framework Main Loop
	while (WM_QUIT != msg.message)
	{
		Input::isAnyKeyDown = false;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//////////////////////////////////////////////////////////////////////
			// FPS locker
			QueryPerformanceCounter(&timeEnd);
			frameTime = static_cast<double>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<double>(timeFreq.QuadPart);
			
			// 
			if (frameTime < minFrameTime) {
				// ミリ秒に変換
				sleepTime = static_cast<DWORD>((minFrameTime - frameTime) * 1000);

				timeBeginPeriod(1);		// 分解能を上げる
				Sleep(sleepTime);
				timeEndPeriod(1);		// 戻す
				// Skip to next frame
				continue;
			}
			timeStart = timeEnd;
			//////////////////////////////////////////////////////////////////////


			Input::KEY = Input::pKeyboard->GetState();
			Input::KEY_TRACKER.Update(Input::KEY);

			// Update GamePads' state
			for (int padIndex = 0; padIndex < Input::MAX_PLAYER_COUNT; ++padIndex) {
				Input::PAD[padIndex] = Input::pGamePad->GetState(padIndex);
				Input::PAD_TRACKER[padIndex].Update(Input::PAD[padIndex]);
			}

			MFAudioCheckLoops();
			if (isFocused)
			{
				Update();
				Draw();
			}

			timer->tick();
			CalculateFrameStats();

			minFrameTime = MIN_FRAME_TIME_DAFAULT;
			if (Input::KEY.LeftControl)
			{
				SetFPSLimitation(5);
			}
			if (Input::KEY.LeftShift)
			{
				SetFPSLimitation(0);
			}
		}

	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Framework::HandleMessage(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(_hwnd, &ps);
		EndPaint(_hwnd, &ps);
		break;
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(msg, wparam, lparam);
		Input::pGamePad->Resume();
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) {
			PostMessage(_hwnd, WM_CLOSE, 0, 0);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(msg, wparam, lparam);
		Input::isAnyKeyDown = true;
		break;
	case WM_DESTROY:
		Input::pGamePad->Suspend();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer->stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer->start();
		break;
	case WM_KILLFOCUS:
		isFocused = false;
		Input::pGamePad->Suspend();
		break;
	case WM_SETFOCUS:
		isFocused = true;
		Input::pGamePad->Resume();
		break;
	case WM_SIZE:
		// Window resizing has already disabled, so WM_SIZE can be realized as FullScreen
		if (wparam == SIZEFULLSCREEN) {
			isFullScreen = !isFullScreen;
		}
		break;
	default:
		return DefWindowProc(_hwnd, msg, wparam, lparam);
	}
	return 0;
}

void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static float sumTime = 0.0f;
	static int frames = 0;
	static float lapsedTime = 0.0f;

	++frames;
	sumTime += frameTime;

	// Compute averages over one second period.
	if ((timer->time_stamp() - lapsedTime) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		static char buf[256] = "";
		sprintf_s(buf, "Untitled FPS: %.0f / FrameTime: %.6f(ms)", fps, frameTime * 1000);
		SetWindowTextA(System::outputWindow, buf);

		// Reset for next average.
		frames = 0;
		lapsedTime += 1.0f;
	}
}

void Framework::Update()
{
	// Update a existed game scene
	// If current scene's next scene was set up, set its next scene as current scene
	if (pScene) {
		pScene->Update();
		if (pScene->pNextScene) {
			pScene = pScene->pNextScene;
		}
	}

}

void Framework::Draw()
{
	// Clear backbuffer and set default depth stencil state
	System::Clear();

	// Alpha blending as default
	Blend::SetMode(System::pImmediateContext, Blend::ALPHA);

	// Render a existed game scene
	if (pScene)
	{
		pScene->Draw();
	}


	// TODO: Achieve Debug Class
	// -5F
	char buf[256];
	sprintf_s(buf, "mainCamera: \nPosX: %f \nPosY: %f \nPosZ: %lf \nDistance: %f \n",
		XMVectorGetX(mainCamera.eyePosition), XMVectorGetY(mainCamera.eyePosition), XMVectorGetZ(mainCamera.eyePosition),
		XMVectorGetX(XMVector3Length(XMVectorSubtract(mainCamera.eyePosition, mainCamera.focusPosition))));

	SpriteString::DrawString(System::pImmediateContext, 0, 0, buf);


	// Swap back buffer to front buffer
	// If this application is full screen, enable the vertical synchronization for better performance
	if (isFullScreen) {
		// vertical synchronization ON
		System::pSwapChain->Present(1, 0);
	}
	else {
		// vertical synchronization OFF
		System::pSwapChain->Present(0, 0);
	}
}

