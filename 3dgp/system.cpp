#include "system.h"

using namespace GLC;

D3D_DRIVER_TYPE             System::driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL           System::featureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*               System::pd3dDevice = nullptr;
ID3D11DeviceContext*        System::pImmediateContext = nullptr;
IDXGISwapChain*             System::pSwapChain = nullptr;
ID3D11RenderTargetView*     System::pRenderTargetView = nullptr;
ID3D11Texture2D*            System::pDepthStencil = nullptr;
ID3D11DepthStencilView*     System::pDepthStencilView = nullptr;
ID3D11DepthStencilState*    System::pDepthStencilState = nullptr;

UINT System::windowWidth;
UINT System::windowHeight;
HWND System::outputWindow;

HRESULT System::Initialize(HWND hWnd)
{
	HRESULT hr = S_OK;
	
	outputWindow = hWnd;

	RECT rc;
	GetClientRect(hWnd, &rc);
	windowWidth = rc.right - rc.left;
	windowHeight = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
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
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pd3dDevice, &featureLevel, &pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = windowWidth;
	descDepth.Height = windowHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	if (FAILED(hr))
		return hr;

	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	hr = pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)windowWidth;
	vp.Height = (FLOAT)windowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &vp);

	UINT m4xMsaaQuality;
	pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM/*DXGI_FORMAT_R32G32B32A32_FLOAT*//*DXGI_FORMAT_R8G8B8A8_UNORM*/, 4, &m4xMsaaQuality);


	return S_OK;
}

void System::Clear()
{
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	pImmediateContext->OMSetDepthStencilState(pDepthStencilState, 1);
}

void System::Release()
{

	if (pDepthStencilState) pDepthStencilState->Release();
	if (pDepthStencilView) pDepthStencilView->Release();
	if (pDepthStencil) pDepthStencil->Release();
	if (pRenderTargetView) pRenderTargetView->Release();
	if (pSwapChain) pSwapChain->Release();

	if (pImmediateContext) pImmediateContext->ClearState();
	if (pImmediateContext) pImmediateContext->Release();
	if (pd3dDevice) pd3dDevice->Release();


}

void System::ReportLiveObjects()
{
#ifdef _DEBUG

	OutputDebugStringA("##############################\n");

	ID3D11Debug* pDebugDevice = nullptr;

	HRESULT hr = pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebugDevice));
	if (FAILED(hr)) {
		MessageBox(0, L"QueryInterface Error", L"System::ReportLiveObjects()", 0);
	}

	hr = pDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	if (FAILED(hr)) {
		MessageBox(0, L"ReportLiveDeviceObjects Error", L"System::ReportLiveObjects()", 0);
	}
	if (pDebugDevice) {
		pDebugDevice->Release();
	}

#endif // _DEBUG
}
