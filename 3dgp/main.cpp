#include "3dgp_system.h"

#include "framework.h"

LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	framework *f = reinterpret_cast<framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->handle_message(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
//#endiferdc
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = fnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("3dgp");
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	//MessageBox(0, L"main.cpp run", L"main", MB_OK);



	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow(_T("3dgp"), _T(""), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	/*HWND hwnd = CreateWindow(_T("3dgp"), _T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);*/
	

	LONG nRet = GetWindowLong(hwnd, GWL_EXSTYLE); 
	nRet = nRet | WS_EX_LAYERED; 
	SetWindowLong(hwnd, GWL_EXSTYLE, nRet);

	float ClearColor[4] = { 0.0f / 255.0f, 111.0f / 255.0f, 129.0f / 255.0f, 1.0f };
	
	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

	//SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 180, LWA_COLORKEY);

	ShowWindow(hwnd, cmd_show);

	framework f(hwnd);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));

	return f.run();
}
