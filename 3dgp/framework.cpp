#include "resources_manager.h"

#include "high_resolution_timer.h"

#include "blend.h"
#include "camera.h"
#include "sprite_string.h"
#include "primitive3d.h"
#include "mf_audio.h"
#include "Scene.h"

#include "input.h"
#include "directxtk.h"

#include "framework.h"
#include <ctime>

using namespace GLC;
using namespace DirectX;


Scene* Framework::pScene = nullptr;

ID3D11Device*           Framework::pDevice = NULL;
ID3D11DeviceContext*    Framework::pDeviceContext = NULL;

ID3D11RenderTargetView*	Framework::pRenderTargetView = NULL;
ID3D11DepthStencilView*	Framework::pDepthStencilView = NULL;

double					Framework::frameTime = 0.0f;

//ID3D11RenderTargetView* framework::s_pRenderTargetView = NULL;

bool Framework::Initialize(HWND _hwnd)
{
	//MessageBox(0, L"Initializer called", L"framework", MB_OK);
	outputWindow = _hwnd;
	DXGI_SWAP_CHAIN_DESC descSwapChain;
	ZeroMemory(&descSwapChain, sizeof(descSwapChain));
	descSwapChain.BufferCount = 1;
	descSwapChain.BufferDesc.Width = SCREEN_WIDTH;
	descSwapChain.BufferDesc.Height = SCREEN_HEIGHT;
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.OutputWindow = outputWindow;
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.SampleDesc.Quality = 0;
	descSwapChain.Windowed = TRUE;
	isFullScreen = !descSwapChain.Windowed;

#ifdef _DEBUG
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
		/*D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1*/
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &descSwapChain, &pSwapChain, &pDevice, &featureLevel, &pDeviceContext);
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		MessageBox(0, L"D3D11CreateDevice Failed.", L"Framework", MB_OK);
		exit(-1);
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		//MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"GetBuffer Failed.", L"Framework", MB_OK);
		return false;
	}

	//D3D11_RENDER_TARGET_VIEW_DESC descRenderTargetView;

	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	if (pBackBuffer) {
		pBackBuffer->Release();
	}
	if (FAILED(hr)) {
		MessageBox(0, L"CreateRenderTargetView Failed.", L"Framework", MB_OK);
		return false;
	}

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepthStencil;
	descDepthStencil.Width = SCREEN_WIDTH;
	descDepthStencil.Height = SCREEN_HEIGHT;
	descDepthStencil.MipLevels = 1;
	descDepthStencil.ArraySize = 1;
	descDepthStencil.Format = DXGI_FORMAT_D24_UNORM_S8_UINT/*DXGI_FORMAT_R32G32B32A32_FLOAT*//*DXGI_FORMAT_R8G8B8A8_UNORM*/;
	descDepthStencil.SampleDesc.Count = 1;
	descDepthStencil.SampleDesc.Quality = 0;
	descDepthStencil.Usage = D3D11_USAGE_DEFAULT;
	descDepthStencil.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepthStencil.CPUAccessFlags = 0;
	descDepthStencil.MiscFlags = 0;

	hr = pDevice->CreateTexture2D(&descDepthStencil, NULL, &pDepthStencilResource);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2D Failed.", L"Framework", MB_OK);
		exit(-1);
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView));
	descDepthStencilView.Format = descDepthStencil.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(pDepthStencilResource, &descDepthStencilView, &pDepthStencilView);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2DResource Failed.", L"Framework", MB_OK);
		return false;
	}
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);


	UINT m4xMsaaQuality;
	pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_R32G32B32A32_FLOAT*//*DXGI_FORMAT_R8G8B8A8_UNORM*/, 4, &m4xMsaaQuality);

	timer = new HighResolutionTimer();


	// Initialzie the blending
	MyBlending::Initialize(pDevice);

	pPrimitive3D[0] = new Primitive3D(pDevice);
	pPrimitive3D[0]->Initialize(pDevice, GEOMETRY_CUBE);
	pPrimitive3D[1] = new Primitive3D(pDevice);
	pPrimitive3D[1]->Initialize(pDevice, GEOMETRY_CYLINDER, 2, 24);

	SpriteString::Initialize(pDevice);
	DXTK::CreateDirectXTKObject(pDevice, pDeviceContext);

	return true;
}

void Framework::setFPSLimitation(int limitation)
{
	if (limitation <= 0)
	{
		minFrameTime = 0;
	}
	else
	{
		minFrameTime = 1.0 / (double)limitation;
	}
}

int Framework::Run()
{
	MSG msg = {};

	/*if (!initialize(hwnd))
	{
	MessageBox(0, L"Run: Iniialize FAILED", 0, 0);
	return 0;
	}*/
	srand((unsigned int)time(NULL));
	for (int padIndex = 0; padIndex < Input::MAX_PLAYER_COUNT; ++padIndex) {
		Input::PAD_TRACKER[padIndex].Reset();
	}

	Input::KEY_TRACKER.Reset();

	//DWORD preTime;

	//////////////////////////////////////////////////////////////////////
	// FPS locker
	DWORD sleepTime;
	if (QueryPerformanceFrequency(&timeFreq) == false)
	{
		MessageBox(0, L"This Device is too old, QueryPerformanceFrequency failed", L"Framework", MB_OK);
		exit(-1);
	}
	QueryPerformanceCounter(&timeStart);
	//////////////////////////////////////////////////////////////////////

	while (WM_QUIT != msg.message)
	{
		//QueryPerformanceFrequency(&timeFreq);
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
			if (frameTime < minFrameTime) { // 時間に余裕がある
				// ミリ秒に変換
				sleepTime = static_cast<DWORD>((minFrameTime - frameTime) * 1000);

				timeBeginPeriod(1);		// 分解能を上げる(こうしないとSleepの精度はガタガタ)
				Sleep(sleepTime);
				timeEndPeriod(1);		// 戻す

										// 次週に持ち越し(こうしないとfpsが変になる?)
				continue;
			}
			timeStart = timeEnd;
			//////////////////////////////////////////////////////////////////////


			Input::KEY = Input::pKeyboard->GetState();
			Input::KEY_TRACKER.Update(Input::KEY);

			for (int padIndex = 0; padIndex < Input::MAX_PLAYER_COUNT; ++padIndex) {
				if (Input::PAD[padIndex].IsConnected()) {
					Input::PAD[padIndex] = Input::pGamePad->GetState(padIndex);
					Input::PAD_TRACKER[padIndex].Update(Input::PAD[padIndex]);
				}
			}
			//DirectX::BasicEffect;

			//preTime = timeGetTime();
			timer->tick();


			MFAudioCheckLoops();
			if (isFocused)
			{
				Update(timer->time_interval());
				Draw(timer->time_interval());
			}

			calculate_frame_stats();

			minFrameTime = MIN_FRAME_TIME_DAFAULT;
			if (Input::KEY.LeftControl)
			{
				setFPSLimitation(5);
			}
			if (Input::KEY.LeftShift)
			{
				setFPSLimitation(0);
			}
		}

	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Framework::handle_message(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//if (wparam == ABN_FULLSCREENAPP){
	//	isFullScreen = !isFullScreen;
	//}
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
		isFullScreen = !isFullScreen;
		break;
	default:
		return DefWindowProc(_hwnd, msg, wparam, lparam);
	}
	return 0;
}

void Framework::calculate_frame_stats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer->time_stamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		static char buf[256] = "";
		sprintf_s(buf, "Untitled FPS: %.0f / FrameTime: %.6f(ms)", fps, frameTime * 1000);
		SetWindowTextA(outputWindow, buf);

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

void Framework::Update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	if (pScene)
	{
		pScene->Update(/*elapsed_time*/);
		if (pScene->pNextScene)
		{
			pScene = pScene->pNextScene;
			//s_pScene->update(/*elapsed_time*/);
		}
	}

	// Charactor Surround Camera for debug
	static float aXY = -XM_PIDIV2, aZY = XM_1DIVPI;
	static float d = XM_PI;
	static bool isCharactorSurroundCameraOn = false;
	static Camera oldCamera;
	if (Input::KEY_TRACKER.pressed.D1)
	{
		isCharactorSurroundCameraOn = !isCharactorSurroundCameraOn;
		if (!isCharactorSurroundCameraOn)
		{
			aXY = -XM_PIDIV2;
			aZY = XM_1DIVPI;
			d = XM_PI;
			GLC::mainCamera = oldCamera;
			//mainCamera = oldCamera;
		}
		else
		{
			oldCamera = mainCamera;
			d = XMVectorGetX(XMVector3Length(mainCamera.focusPosition - mainCamera.eyePosition));
		}
	}
	if (isCharactorSurroundCameraOn)
	{
		if (Input::KEY.J) {
			aXY -= 0.01f;
		}
		if (Input::KEY.L) {
			aXY += 0.01f;
		}
		if (Input::KEY.I) {
			aZY += 0.01f;
		}
		if (Input::KEY.K) {
			aZY -= 0.01f;
		}
		if (Input::KEY.O) {
			d += 0.05f;
		}
		if (Input::KEY.U) {
			d -= 0.05f;
		}
		//mainCamera.eyePosition = mainCamera.eyePosition*XMQuaternionRotationRollPitchYaw(aXY, aZY, 0);
		mainCamera.eyePosition = XMVectorSet(-fabs(d)*cosf(aZY)*sinf(aXY), fabs(d)*sinf(aZY), -fabs(d)*cosf(aZY)*cosf(aXY), 0);


		XMVECTOR forword = XMVectorSubtract(mainCamera.focusPosition, mainCamera.eyePosition);
		forword = XMVector3Normalize(forword);
		float angle = XMVectorGetX(XMVector3Dot(forword, { 0,1,0 }));
		mainCamera.upDirection = XMLoadFloat3(&XMFLOAT3(0, 1, 0)) - forword*angle;

		mainCamera.upDirection = XMVector3Normalize(mainCamera.upDirection);

		//mainCamera.upDirection = { sinf(aZY)*sinf(aXY), cosf(aZY), sinf(aZY)*cosf(aXY), 0 };
		
	}

}

void Framework::Draw(float elapsed_time/*Elapsed seconds from last frame*/)
{
	//Camera::mainCamera.focusPosition = { focusPos.x,focusPos.y,focusPos.z,0 };

	// Change the blending mode 
	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		blendMode++;
		blendMode %= 9;
	}
	MyBlending::setMode(pDeviceContext, BLEND_ALPHA);

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	mainCamera.viewPort.Width = SCREEN_WIDTH;
	mainCamera.viewPort.Height = SCREEN_HEIGHT;
	mainCamera.Update();
	pDeviceContext->RSSetViewports(1, &mainCamera.viewPort);

	// Just clear the backbuffer
	//float ClearColor[4] = { 0.0f / 255.0f, 111.0f / 255.0f, 129.0f / 255.0f, 1.0f }; //red,green,blue,alpha
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	MyBlending::setMode(pDeviceContext, BLEND_ALPHA);


	if (pScene)
	{
		pScene->Draw(/*elapsed_time*/);
	}

	MyBlending::setMode(pDeviceContext, BLEND_NONE);

	// -5F
	char buf[256];
	sprintf_s(buf, "mainCamera: \nPosX: %f \nPosY: %f \nPosZ: %lf \nDistance: %f \n",
		XMVectorGetX(mainCamera.eyePosition), XMVectorGetY(mainCamera.eyePosition), XMVectorGetZ(mainCamera.eyePosition),
		XMVectorGetX(XMVector3Length(XMVectorSubtract(mainCamera.eyePosition, mainCamera.focusPosition))));


	MyBlending::setMode(pDeviceContext, BLEND_ALPHA);
	SpriteString::DrawString(pDeviceContext, 0, 0, buf);

	if (isFullScreen) {
		// 垂直同期ON
		pSwapChain->Present(1, 0);
	}
	else {
		// 垂直同期OFF
		pSwapChain->Present(0, 0);
	}

	
	// For FullScreen Mode, Synchronize presentation for 1 vertical blanks
	//pSwapChain->Present(1, 0);

	// For Windowed Mode 
	//pSwapChain->Present(0, 0);
}

void Framework::Release()
{
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);

	if (pDeviceContext) {
		pDeviceContext->ClearState();
		//MessageBox(0, L"DeviceContext Cleared", L"framework", MB_OK);
		pDeviceContext->Release();
		//MessageBox(0, L"DeviceContext Released", L"framework", MB_OK);
	}

	
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pDepthStencilResource);
	SAFE_RELEASE(pDepthStencilState);
	
	delete timer;

	MyBlending::Release();

	SpriteString::Release();
	DXTK::CleanupDirectXTKObject();

	for (auto &p : pPrimitive3D)
	{
		delete p;
		p = NULL;
	}
}
