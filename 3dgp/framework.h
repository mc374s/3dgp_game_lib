#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#pragma comment(lib, "winmm")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "DirectXTK")

#include <d3d11.h>


#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#define FPS (60)


class HighResolutionTimer;
class Scene;

class Framework
{
private:
	static Scene* pScene;

	const double MIN_FRAME_TIME_DAFAULT = 1.0 / FPS;
	double minFrameTime = MIN_FRAME_TIME_DAFAULT;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	HighResolutionTimer* timer;

	void CalculateFrameStats();

	bool Initialize(HWND hWnd);
	void Update();
	void Draw();
	void Release();

public:
	Framework(HWND hWnd);
	~Framework();

	static double deltaTime;

	bool isFullScreen;
	bool isFocused = true;


	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	int Run();

	void ChangeScene(Scene* pNextScene);

	// Set limitation to 0 fps means no limitation
	void SetFPSLimitation(int limitation = FPS);
};


#endif // !_FRAMEWORK_H_