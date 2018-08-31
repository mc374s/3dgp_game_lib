#include "framework.h"
#include "blend.h"
#include "sprite_string.h"
#include "mf_audio.h"

enum AUDIO_NO
{
	SPACE
};

RESOURCE_DATA audioFile[]{
	{ 0,"spaceinvaders1.mp3"},
	{ 1,"shoot.wav" },
	{-1,nullptr}
};

CameraData e_CameraData;

//ID3D11RenderTargetView* framework::m_pRenderTargetView = NULL;

framework::framework(HWND hwnd) /*: hwnd(hwnd)*/
{
	//MessageBox(0, L"Constructor called", L"framework", MB_OK);
	if (!initialize(hwnd))
	{
		MessageBox(0, L"run: Iniialize FAILED", 0, 0);
		return;
	}
}

framework::~framework()
{
	release();
}

void framework::setFPSLimitation(int a_FPS)
{
	m_minFrameTime = 1.0f / a_FPS;
}

int framework::run()
{
	MSG msg = {};

	//////////////////////////////////////////////////////////////////////
	// FPS locker
	DWORD sleepTime;
	if (QueryPerformanceFrequency(&m_timeFreq) == false)
	{
		MessageBox(0, L"This Device is too old, QueryPerformanceFrequency failed", L"framework", MB_OK);
		exit(-1);
	}
	QueryPerformanceCounter(&m_timeStart);
	//////////////////////////////////////////////////////////////////////

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//////////////////////////////////////////////////////////////////////
			// FPS locker
			QueryPerformanceCounter(&m_timeEnd);
			m_frameTime = static_cast<float>(m_timeEnd.QuadPart - m_timeStart.QuadPart) / static_cast<float>(m_timeFreq.QuadPart);
			if (m_frameTime < m_minFrameTime) { // 時間に余裕がある
												// ミリ秒に変換
				sleepTime = static_cast<DWORD>((m_minFrameTime - m_frameTime) * 1000);

				timeBeginPeriod(1);		// 分解能を上げる(こうしないとSleepの精度はガタガタ)
				Sleep(sleepTime);
				timeEndPeriod(1);		// 戻す

										// 次週に持ち越し(こうしないとfpsが変になる?)
				continue;
			}
			m_timeStart = m_timeEnd;

			m_minFrameTime = MIN_FRAME_TIME_DAFAULT;
			if (GetAsyncKeyState(VK_SHIFT) < 0)
			{
				setFPSLimitation(2000);
			}
			if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				setFPSLimitation(5);
			}
			//////////////////////////////////////////////////////////////////////

			timer.tick();
			calculate_frame_stats();
			update(timer.time_interval());
			render(timer.time_interval());

		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK framework::handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.start();
		break;
	case WM_LBUTTONDOWN:
		//PostMessage(hwnd, WM_CLOSE, 0, 0);
		//PostMessage(hwnd, WM_NCLBUTTONDOWN, 0, 0);
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
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
	if ((timer.time_stamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";

		outs.precision(4);
		outs << " #Blending Mode: " << strBlendMode[blendMode] << " #Alpha: " << alpha << " / 255.0f ( " << alpha / 255.0f * 100 << "% )";
		SetWindowTextA(m_hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}


bool framework::initialize(HWND hwnd)
{
	//MessageBox(0, L"Initializer called", L"framework", MB_OK);
	m_hWnd = hwnd;
	DXGI_SWAP_CHAIN_DESC descSwapChain;
	ZeroMemory(&descSwapChain, sizeof(descSwapChain));
	descSwapChain.BufferCount = 1;
	descSwapChain.BufferDesc.Width = SCREEN_WIDTH;
	descSwapChain.BufferDesc.Height = SCREEN_HEIGHT;
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.OutputWindow = m_hWnd;
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.SampleDesc.Quality = 0;
	descSwapChain.Windowed = TRUE;

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
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &descSwapChain, &m_pSwapChain, &m_pDevice, &m_featureLevel, &m_pDeviceContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)){
		//MessageBox(0, L"D3D11CreateDevice Failed.", L"framework", MB_OK);
		return false;
	}
	if (m_featureLevel!=D3D_FEATURE_LEVEL_11_0){
		//MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"GetBuffer Failed.", L"framework", MB_OK);
		return false;
	}

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	if (pBackBuffer){
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
	descDepthStencil.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepthStencil.SampleDesc.Count = 1;
	descDepthStencil.SampleDesc.Quality = 0;
	descDepthStencil.Usage = D3D11_USAGE_DEFAULT;
	descDepthStencil.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepthStencil.CPUAccessFlags = 0;
	descDepthStencil.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&descDepthStencil, NULL, &m_pDepthStencilResource);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2D Failed.", L"framework", MB_OK);
		return false;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView));
	descDepthStencilView.Format = descDepthStencil.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilResource, &descDepthStencilView, &m_pDepthStencilView);
	if (FAILED(hr)) {
		MessageBox(0, L"CreateTexture2DResource Failed.", L"framework", MB_OK);
		return false;
	}
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);


	UINT m4xMsaaQuality;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

	// Initialzie the blending
	MyBlending::initialize(m_pDevice);	

	/*for (auto &p : sprites)
	{
		p = new Sprite(m_pd3dDevice, L"player-sprites.png");
	}*/

	m_pSprites[0] = new Sprite(m_pDevice, "page_left.png", true);
	m_pSprites[1] = new Sprite(m_pDevice, "page_right.png");
	m_pSprites[2] = new Sprite(m_pDevice, "cover_left.png");
	m_pSprites[3] = new Sprite(m_pDevice, "cover_right.png");

	m_pPrimitive3D[0] = new Primitive3D(m_pDevice);
	m_pPrimitive3D[0]->initialize(m_pDevice, GEOMETRY_CUBE);
	m_pPrimitive3D[1] = new Primitive3D(m_pDevice);
	m_pPrimitive3D[1]->initialize(m_pDevice, GEOMETRY_CYLINDER, 2, 10);

	m_pMesh[0] = new SkinnedMesh(m_pDevice, "DATA/000_cube.fbx",true);
	m_pMesh[1] = new SkinnedMesh(m_pDevice, "DATA/001_cube.fbx",true);
	m_pMesh[2] = new SkinnedMesh(m_pDevice, "DATA/002_cube.fbx",true);
	m_pMesh[3] = new SkinnedMesh(m_pDevice, "DATA/003_cube.fbx",true);
	m_pMesh[4] = new SkinnedMesh(m_pDevice, "DATA/004_cube.fbx", true);
	m_pMesh[5] = new SkinnedMesh(m_pDevice, "DATA/005_cube.fbx", true);
	m_pMesh[6] = new SkinnedMesh(m_pDevice, "DATA/danbo_fbx/danbo_atk.fbx",true);

	initSprString(m_pDevice);

	//pMFAudioManager->loadAudios(audioFile);
	

	return true;
}

void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	
}

void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	using namespace DirectX;
	// Just clear the backbuffer

	LARGE_INTEGER performance, performanceCounterStart = { 0 }, performanceCounterEnd = { 0 };

	QueryPerformanceFrequency(&performance);
	QueryPerformanceCounter(&performanceCounterStart);

	float ClearColor[4] = { 0.0f / 255.0f, 111.0f / 255.0f, 129.0f / 255.0f, 1.0f }; //red,green,blue,alpha
	//float ClearColor[4] = { 0, 0, 0, 0.0f }; //red,green,blue,alpha
	//float ClearColor[4] = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f }; //red,green,blue,alpha
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Test variables
	static double time;
	static CUSTOM3D custom3DTemp;
	static XMFLOAT3 position(0, 0, 0);
	static float mX = 0, mY = 0, mZ = 0;
	static float aXY = 0.0f, aZY = 0.0f;
	static float d = 2*XM_2PI;
	static float hClosed = -2.0f;
	static XMFLOAT3 focusPos = { 0,0/* + 310 / (float)SCREEN_WIDTH*/,0 };
	if (GetAsyncKeyState('J') < 0) {
		aXY -= 0.01f;
	}
	if (GetAsyncKeyState('L') < 0) {
		aXY += 0.01f;
	}
	if (GetAsyncKeyState('I') < 0) {
		aZY += 0.01f;
	}
	if (GetAsyncKeyState('K') < 0) {
		aZY -= 0.01f;
	}
	if (GetAsyncKeyState('O') < 0) {
		d += 0.05f;
	}
	if (GetAsyncKeyState('U') < 0) {
		d -= 0.05f;
	}
	e_CameraData.upDirection = { sinf(aZY)*sinf(aXY), cosf(aZY), sinf(aZY)*cosf(aXY), 0 };
	e_CameraData.eyePosition = { -fabs(d)*cosf(aZY)*sinf(aXY), fabs(d)*sinf(aZY)/* + 310 / (float)SCREEN_WIDTH*/, -fabs(d)*cosf(aZY)*cosf(aXY),0 };

	if (GetAsyncKeyState('1') < 0) {
		e_CameraData.eyePosition = { 2, 0, 0, 0 };
	}
	if (GetAsyncKeyState('2') < 0) {
		e_CameraData.upDirection = { 0, 0, 1, 0 };
		e_CameraData.eyePosition = { 0, 1, 0, 0 };
	}
	if (GetAsyncKeyState('3') < 0) {
		e_CameraData.eyePosition = { 0, 0, 2, 0 };
	}
	if (GetAsyncKeyState('0') < 0) {
		mX = mY = mZ = 0;
		//a = 0.0f;
		//h = -0.15f;
		custom3DTemp.clear();
	}

	if (GetAsyncKeyState('W') < 0) {
		custom3DTemp.angleYawPitchRoll.y += 1;
	}
	if (GetAsyncKeyState('S') < 0) {
		custom3DTemp.angleYawPitchRoll.y -= 1;
	}
	if (GetAsyncKeyState('A') < 0) {
		custom3DTemp.angleYawPitchRoll.x += 1;
	}
	if (GetAsyncKeyState('D') < 0) {
		custom3DTemp.angleYawPitchRoll.x -= 1;
	}
	if (GetAsyncKeyState('Q') < 0) {
		custom3DTemp.angleYawPitchRoll.z += 1;
	}
	if (GetAsyncKeyState('E') < 0) {
		custom3DTemp.angleYawPitchRoll.z -= 1;
	}
	if (GetAsyncKeyState('Z') < 0) {
		position.x -= 10;
	}
	if (GetAsyncKeyState('X') < 0) {
		position.x += 10;
	}
	if (GetAsyncKeyState('C') < 0) {
		position.y += 10;
	}
	if (GetAsyncKeyState('V') < 0) {
		position.y -= 10;
	}
	if (GetAsyncKeyState('B') < 0) {
		position.z += 10;
	}
	if (GetAsyncKeyState('N') < 0) {
		position.z -= 10;
	}

	e_CameraData.focusPosition = { focusPos.x,focusPos.y,focusPos.z,0 };

	// Change the blending mode 
	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		blendMode++;
		blendMode %= 9;
	}
	MyBlending::setMode(m_pDeviceContext, BLEND_ALPHA);
	m_pPrimitive3D[0]->drawCube(m_pDeviceContext, XMFLOAT3(1024, 0, 0), XMFLOAT3(2048, 2, 2));
	m_pPrimitive3D[0]->drawCube(m_pDeviceContext, XMFLOAT3(0, 1024, 0), XMFLOAT3(2, 2048, 2));
	m_pPrimitive3D[0]->drawCube(m_pDeviceContext, XMFLOAT3(0, 0, 1024), XMFLOAT3(2, 2, 2048));

	//m_pPrimitive3D[0]->drawCube(m_pDeviceContext, XMFLOAT3(-310, mY, 10 + mZ), XMFLOAT3(620, 700, 20), &custom3DTemp);
	//m_pPrimitive3D[0]->drawCube(m_pDeviceContext, XMFLOAT3(-500, 0, 0), XMFLOAT3(500, 500, 500), &custom3DTemp);
	//m_pPrimitive3D[1]->drawCylinder(m_pDeviceContext, XMFLOAT3(310, mY, 10 + mZ), XMFLOAT3(620, 700, 20), &custom3DTemp);

	//custom3DTemp.position.x -= SCREEN_WIDTH / 2;
	//custom3DTemp.scaling.x *= 70;
	//custom3DTemp.scaling.y = custom3DTemp.scaling.z = custom3DTemp.scaling.x;

	MyBlending::setMode(m_pDeviceContext, BLEND_ALPHA);
	//m_pSprites[0]->render3D(m_pDeviceContext, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 620, 700, 0, 0xFFFFFFFF, 0, 0, &custom3DTemp);
	custom3DTemp.position.x = (-1280 - position.x) * 6;
	custom3DTemp.position.y = 0;
	custom3DTemp.position.z = 0;
	m_pMesh[0]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp);

	custom3DTemp.position.x = (-1280 - position.x) * 4;
	m_pMesh[1]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp);
	

	custom3DTemp.position.x = (-1280 - position.x) * 2;
	m_pMesh[2]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp);

	custom3DTemp.position.x = 0;
	m_pMesh[3]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp);


	custom3DTemp.position.x = (1280 + position.x) * 2;
	m_pMesh[4]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp);

	custom3DTemp.position.x = (1280 + position.x) * 4;
	m_pMesh[5]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp);

	custom3DTemp.scaling = { 0.05f,0.05f,0.05f };
	custom3DTemp.position.x = (1280 + position.x) * 6;
	custom3DTemp.position.y = position.y * 5;
	m_pMesh[6]->drawMesh(m_pDeviceContext, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), &custom3DTemp, elapsed_time);
	custom3DTemp.position.y = 0;
	custom3DTemp.scaling = { 1,1,1 };

	
	//m_pSprites[0]->render3D(m_pDeviceContext, 0, 0, 620, 700, 0, 0, 620, 700, 0, 0xFFFFFFFF, 0, 0, &custom3DTemp);

	//custom3DTemp.scaling.x /= 70;
	/*Scustom3DTemp.scaling.y = custom3DTemp.scaling.z = custom3DTemp.scaling.x;
	m_pMesh[2]->drawMesh(m_pDeviceContext, XMFLOAT3(310, mY, 10 + mZ), XMFLOAT3(620, 700, 20), &custom3DTemp);*/

	/*custom3DTemp.angleYawPitchRoll.x = -custom3DTemp.angleYawPitchRoll.x;
	m_pPrimitive3D[0]->drawCube(m_pDeviceContext, XMFLOAT3(310, mY, 10 + mZ), XMFLOAT3(620, 700, 20), &custom3DTemp);
	custom3DTemp.angleYawPitchRoll.x = -custom3DTemp.angleYawPitchRoll.x;*/

	//custom3DTemp.position.z -= 0.1;
	//m_pSprites[0]->render3D(m_pDeviceContext, -620, -700 / 2, 620, 700, 620, 0, 620, 700, 0, 0xFFFFFF00, 0, 0, &custom3DTemp);
	//custom3DTemp.position.z += 0.1;
	
	
	//sprites[0]->render3D(m_pDeviceContext, 0, -SCREEN_HEIGHT / 2, 620, SCREEN_HEIGHT, 620, 0, 620, SCREEN_HEIGHT, 0, 0xFFFFFF00, 0, 0, &custom3DTemp);

	char buf[256];
	sprintf_s(buf, "posX: %lf \nposY: %lf \nposZ: %lf \ncameraDistance: %lf \n", custom3DTemp.position.x, custom3DTemp.position.y, custom3DTemp.position.z, d);
	MyBlending::setMode(m_pDeviceContext, BLEND_ALPHA);
	drawSprString(m_pDeviceContext, 0, 0, buf);
	//sprintf_s(buf, "PerformanceFrequency %I64u \nPerformanceCounter %I64u\n%lfmspf", performance.QuadPart, max.QuadPart, time);
	//drawSprString(m_pDeviceContext, -SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2 + 560, buf);

	

	QueryPerformanceCounter(&performanceCounterEnd);
	frame[frameCounter].QuadPart = performanceCounterEnd.QuadPart - performanceCounterStart.QuadPart;
	frameCounter++;
	if (frameCounter > N)
	{ 
		frameCounter = 0;
	}

	for (int i = 0; i < N; i++)
	{
		if (frame[i].QuadPart > max.QuadPart)
		{
			max.QuadPart = frame[i].QuadPart;
		}
	}

	time = (double)max.QuadPart / (double)performance.QuadPart * 1000;
	
	m_pSwapChain->Present(0, 0);
}

void framework::release()
{

	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		//MessageBox(0, L"DepthStencilView Released", L"framework", MB_OK);
	}
	if (m_pRenderTargetView) {
		m_pRenderTargetView->Release();
		//MessageBox(0, L"RenderTargetView Released", L"framework", MB_OK);
	}
	if (m_pSwapChain) {
		m_pSwapChain->Release();
		//MessageBox(0, L"SwapChain Released", L"framework", MB_OK);
	}
	if (m_pDeviceContext) {
		m_pDeviceContext->ClearState();
		//MessageBox(0, L"DeviceContext Cleared", L"framework", MB_OK);
		m_pDeviceContext->Release();
		//MessageBox(0, L"DeviceContext Released", L"framework", MB_OK);
	}
	if (m_pDevice) {
		m_pDevice->Release();
		//MessageBox(0, L"Device Released", L"framework", MB_OK);
	}

	if (m_pBlendState) m_pBlendState->Release();

	if (m_pDepthStencilResource) {
		m_pDepthStencilResource->Release();
	}

	MyBlending::release();

	for (auto &p : m_pMesh)
	{
		delete p;
		p = NULL;
	}
	for (auto &p : m_pPrimitive3D)
	{
		delete p;
		p = NULL;
	}
	for (auto &p : m_pSprites)
	{
		delete p;
		p = NULL;
	}

	releaseSprString();

	if (m_pMFPlayer)
	{
		m_pMFPlayer->Release();
		m_pMFPlayer = nullptr;
	}
}