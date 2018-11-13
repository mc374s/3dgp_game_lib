#include "resources_manager.h"

#include "high_resolution_timer.h"

#include "blend.h"
#include "camera.h"
#include "sprite_string.h"
#include "primitive3d.h"
#include "mf_audio.h"
#include "Scene.h"

#include "framework.h"
#include <ctime>

std::unique_ptr<DirectX::Keyboard> e_pKeyboard = std::make_unique<Keyboard>();
DirectX::Keyboard::State KEY_BOARD = Keyboard::State();
DirectX::Keyboard::KeyboardStateTracker KEY_TRACKER = DirectX::Keyboard::KeyboardStateTracker();

std::unique_ptr<DirectX::GamePad> e_pGamePad = std::make_unique<GamePad>();
DirectX::GamePad::State GAME_PAD = GamePad::State();
DirectX::GamePad::ButtonStateTracker PAD_TRACKER = DirectX::GamePad::ButtonStateTracker();

bool e_isAnyKeyDown = false;

Scene* framework::s_pScene = nullptr;

ID3D11Device*           framework::s_pDevice = NULL;
ID3D11DeviceContext*    framework::s_pDeviceContext = NULL;

ID3D11RenderTargetView*	framework::s_pRenderTargetView = NULL;
ID3D11DepthStencilView*	framework::s_pDepthStencilView = NULL;

int e_renderTargetWidth = SCREEN_WIDTH;
int e_renderTargetHeight = SCREEN_HEIGHT;

//ID3D11RenderTargetView* framework::s_pRenderTargetView = NULL;

bool framework::Initialize(HWND _hwnd)
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
			D3D11_SDK_VERSION, &descSwapChain, &pSwapChain, &s_pDevice, &featureLevel, &s_pDeviceContext);
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		MessageBox(0, L"D3D11CreateDevice Failed.", L"framework", MB_OK);
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
		MessageBox(0, L"GetBuffer Failed.", L"framework", MB_OK);
		return false;
	}

	//D3D11_RENDER_TARGET_VIEW_DESC descRenderTargetView;

	hr = s_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &s_pRenderTargetView);
	if (pBackBuffer) {
		pBackBuffer->Release();
	}
	if (FAILED(hr)) {
		MessageBox(0, L"CreateRenderTargetView Failed.", L"framework", MB_OK);
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

	hr = s_pDevice->CreateTexture2D(&descDepthStencil, NULL, &pDepthStencilResource);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2D Failed.", L"framework", MB_OK);
		exit(-1);
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView));
	descDepthStencilView.Format = descDepthStencil.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0;
	hr = s_pDevice->CreateDepthStencilView(pDepthStencilResource, &descDepthStencilView, &s_pDepthStencilView);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2DResource Failed.", L"framework", MB_OK);
		return false;
	}
	s_pDeviceContext->OMSetRenderTargets(1, &s_pRenderTargetView, s_pDepthStencilView);

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = s_pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	s_pDeviceContext->RSSetViewports(1, &vp);


	UINT m4xMsaaQuality;
	s_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_R32G32B32A32_FLOAT*//*DXGI_FORMAT_R8G8B8A8_UNORM*/, 4, &m4xMsaaQuality);

	timer = new HighResolutionTimer();


	// Initialzie the blending
	MyBlending::Initialize(s_pDevice);

	pPrimitive3D[0] = new Primitive3D(s_pDevice);
	pPrimitive3D[0]->Initialize(s_pDevice, GEOMETRY_CUBE);
	pPrimitive3D[1] = new Primitive3D(s_pDevice);
	pPrimitive3D[1]->Initialize(s_pDevice, GEOMETRY_CYLINDER, 2, 24);

	SpriteString::Initialize(s_pDevice);

	return true;
}

void framework::setFPSLimitation(int limitation)
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

int framework::Run()
{
	MSG msg = {};

	/*if (!initialize(hwnd))
	{
	MessageBox(0, L"Run: Iniialize FAILED", 0, 0);
	return 0;
	}*/
	srand((unsigned int)time(NULL));

	PAD_TRACKER.Reset();

	KEY_TRACKER.Reset();

	//DWORD preTime;

	//////////////////////////////////////////////////////////////////////
	// FPS locker
	DWORD sleepTime;
	if (QueryPerformanceFrequency(&timeFreq) == false)
	{
		MessageBox(0, L"This Device is too old, QueryPerformanceFrequency failed", L"framework", MB_OK);
		exit(-1);
	}
	QueryPerformanceCounter(&timeStart);
	//////////////////////////////////////////////////////////////////////

	while (WM_QUIT != msg.message)
	{
		//QueryPerformanceFrequency(&timeFreq);
		e_isAnyKeyDown = false;
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


			KEY_BOARD = e_pKeyboard->GetState();
			KEY_TRACKER.Update(KEY_BOARD);

			GAME_PAD = e_pGamePad->GetState(0);
			PAD_TRACKER.Update(GAME_PAD);

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
			if (KEY_BOARD.LeftControl)
			{
				setFPSLimitation(5);
			}
			if (KEY_BOARD.LeftShift)
			{
				setFPSLimitation(0);
			}
		}

	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK framework::handle_message(HWND _hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//if (wparam == ABN_FULLSCREENAPP){
	//	isFullScreen = !isFullScreen;
	//}
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(_hwnd, &ps);
		EndPaint(_hwnd, &ps);
		break;
	}
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(msg, wparam, lparam);
		e_pGamePad->Resume();
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) {
			PostMessage(_hwnd, WM_CLOSE, 0, 0);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(msg, wparam, lparam);
		e_isAnyKeyDown = true;
		break;
	case WM_DESTROY:
		e_pGamePad->Suspend();
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
		e_pGamePad->Suspend();
		break;
	case WM_SETFOCUS:
		isFocused = true;
		e_pGamePad->Resume();
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

void framework::calculate_frame_stats()
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

void framework::Update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	if (s_pScene)
	{
		s_pScene->Update(/*elapsed_time*/);
		if (s_pScene->pNextScene)
		{
			s_pScene = s_pScene->pNextScene;
			//s_pScene->update(/*elapsed_time*/);
		}
	}

	// Charactor Surround Camera for debug
	static float aXY = -XM_PIDIV2, aZY = XM_1DIVPI;
	static float d = XM_PI;
	static bool isCharactorSurroundCameraOn = false;
	static Camera oldCamera;
	if (KEY_TRACKER.pressed.D1)
	{
		isCharactorSurroundCameraOn = !isCharactorSurroundCameraOn;
		if (!isCharactorSurroundCameraOn)
		{
			aXY = -XM_PIDIV2;
			aZY = XM_1DIVPI;
			d = XM_PI;
			Camera::mainCamera = oldCamera;
		}
		else
		{
			oldCamera = Camera::mainCamera;
		}
	}
	if (isCharactorSurroundCameraOn)
	{
		if (KEY_BOARD.J) {
			aXY -= 0.01f;
		}
		if (KEY_BOARD.L) {
			aXY += 0.01f;
		}
		if (KEY_BOARD.I) {
			aZY += 0.01f;
		}
		if (KEY_BOARD.K) {
			aZY -= 0.01f;
		}
		if (KEY_BOARD.O) {
			d += 0.05f;
		}
		if (KEY_BOARD.U) {
			d -= 0.05f;
		}
		Camera::mainCamera.upDirection = { sinf(aZY)*sinf(aXY), cosf(aZY), sinf(aZY)*cosf(aXY), 0 };
		Camera::mainCamera.eyePosition = XMVectorSet(-fabs(d)*cosf(aZY)*sinf(aXY), fabs(d)*sinf(aZY)/* + 310 / (float)SCREEN_WIDTH*/, -fabs(d)*cosf(aZY)*cosf(aXY), 0) + oldCamera.eyePosition;
	}

}

void framework::Draw(float elapsed_time/*Elapsed seconds from last frame*/)
{
	//Camera::mainCamera.focusPosition = { focusPos.x,focusPos.y,focusPos.z,0 };

	// Change the blending mode 
	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		blendMode++;
		blendMode %= 9;
	}
	MyBlending::setMode(s_pDeviceContext, BLEND_ALPHA);

	s_pDeviceContext->OMSetRenderTargets(1, &s_pRenderTargetView, s_pDepthStencilView);

	Camera::mainCamera.viewPort.Width = SCREEN_WIDTH;
	Camera::mainCamera.viewPort.Height = SCREEN_HEIGHT;
	s_pDeviceContext->RSSetViewports(1, &Camera::mainCamera.viewPort);

	// Just clear the backbuffer
	//float ClearColor[4] = { 0.0f / 255.0f, 111.0f / 255.0f, 129.0f / 255.0f, 1.0f }; //red,green,blue,alpha
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha
	s_pDeviceContext->ClearRenderTargetView(s_pRenderTargetView, ClearColor);
	s_pDeviceContext->ClearDepthStencilView(s_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	s_pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

	MyBlending::setMode(s_pDeviceContext, BLEND_ALPHA);
	if (s_pScene)
	{
		s_pScene->Draw(/*elapsed_time*/);
	}

	MyBlending::setMode(s_pDeviceContext, BLEND_NONE);

	
	//pPrimitive3D[0]->Draw(s_pDeviceContext, XMMatrixIdentity(), XMFLOAT3(2.0f, 0.01f, 0.01f), 0xFF0000FF);
	//pPrimitive3D[0]->Draw(s_pDeviceContext, XMMatrixIdentity(), XMFLOAT3(0.01f, 2.0f, 0.01f), 0x00FF00FF);
	//spPrimitive3D[0]->Draw(s_pDeviceContext, XMMatrixIdentity(), XMFLOAT3(0.01f, 0.01f, 2.0f), 0x0000FFFF);
	//pPrimitive3D[1]->drawCylinder(s_pDeviceContext, XMFLOAT3(-310, 0, 10 + 0), XMFLOAT3(620, 700, 20), &custom3DTemp);

	// -5F
	char buf[256];
	sprintf_s(buf, "mainCamera: \nPosX: %f \nPosY: %f \nPosZ: %lf \nDistance: %f \n",
		Camera::mainCamera.eyePosition.m128_f32[0], Camera::mainCamera.eyePosition.m128_f32[1], Camera::mainCamera.eyePosition.m128_f32[2],
		XMVector3Length(XMVectorSubtract(Camera::mainCamera.eyePosition, Camera::mainCamera.focusPosition)).m128_f32[0]);

	if (!XMVerifyCPUSupport())
	{
		exit(-5);
	}
	MyBlending::setMode(s_pDeviceContext, BLEND_ALPHA);
	SpriteString::DrawString(s_pDeviceContext, 0, 0, buf);

	if (KEY_TRACKER.pressed.I)
	{
		isFullScreen = !isFullScreen;
	}

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

void framework::Release()
{
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(s_pDepthStencilView);
	SAFE_RELEASE(s_pRenderTargetView);
	SAFE_RELEASE(pSwapChain);

	if (s_pDeviceContext) {
		s_pDeviceContext->ClearState();
		//MessageBox(0, L"DeviceContext Cleared", L"framework", MB_OK);
		s_pDeviceContext->Release();
		//MessageBox(0, L"DeviceContext Released", L"framework", MB_OK);
	}

	
	SAFE_RELEASE(s_pDevice);
	SAFE_RELEASE(pDepthStencilResource);
	SAFE_RELEASE(pDepthStencilState);
	
	delete timer;

	MyBlending::Release();

	SpriteString::Release();

	for (auto &p : pPrimitive3D)
	{
		delete p;
		p = NULL;
	}
}
