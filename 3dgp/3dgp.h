#ifndef  _3DGP_H_
#define  _3DGP_H_

#pragma comment(lib,"d3d11")
#pragma comment(lib,"dxgi")
#pragma comment(lib,"DirectXTK")

#include <windows.h>
#include <memory>
#include <d3d11.h>
#include <tchar.h>
#include <assert.h>
#include <tchar.h>
#include <ctime>
#include <cstdlib>
#include <vector>


#include <DirectXMath.h>

#include "Keyboard.h"
#include "GamePad.h"

using namespace DirectX;


#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

#include "high_resolution_timer.h"

#include "resources_manager.h"

#include "camera.h"

extern Camera e_mainCamera;

extern std::unique_ptr<DirectX::Keyboard> e_pKeyboard;
extern DirectX::Keyboard::State KEY_BOARD;
extern DirectX::Keyboard::KeyboardStateTracker KEY_TRACKER;

extern std::unique_ptr<DirectX::GamePad> e_pGamePad;
extern DirectX::GamePad::State GAME_PAD;
extern DirectX::GamePad::ButtonStateTracker PAD_TRACKER;

extern bool e_isAnyKeyDown;



#endif // ! _3DGP_H_
