#include "framework.h"

#include "../Game/game.h"
#include "../Game/sound_data.h"
#include "../Game/sprite_data.h"
#include "../Game/scene_title.h"
#include <thread>
#include <tchar.h>

using namespace GLC;

LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Framework *f = reinterpret_cast<Framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
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

	MFAudioManager::getInstance()->loadAudios(audio_data);

	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow(_T("3dgp"), _T(""), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	/*HWND hwnd = CreateWindow(_T("3dgp"), _T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);*/
	

	LONG nRet = GetWindowLong(hwnd, GWL_EXSTYLE); 
	nRet = nRet | WS_EX_LAYERED; 
	SetWindowLong(hwnd, GWL_EXSTYLE, nRet);

	
	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

	//SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 180, LWA_COLORKEY);
	


	Framework f(hwnd);
	srand(unsigned int(time(NULL)));
	pTextureManager->LoadTextures(e_loadTexture);		// 2D画像の一括ロード

	Framework::changeScene(SCENE_TITLE);


	ShowWindow(hwnd, cmd_show);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));

	return f.Run();
}
