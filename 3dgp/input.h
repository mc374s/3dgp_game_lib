#ifndef _INPUT_H_
#define _INPUT_H_


#include <windows.h>

#include "Keyboard.h"
#include "GamePad.h"
using namespace DirectX;

class Input {

public:
	static std::unique_ptr<Keyboard>			pKeyboard;
	static Keyboard::State						KEY_BOARD;
	static Keyboard::KeyboardStateTracker		KEY_TRACKER;

	static std::unique_ptr<DirectX::GamePad>	pGamePad;
	static GamePad::State						GAME_PAD;
	static GamePad::ButtonStateTracker			PAD_TRACKER;

	static bool isAnyKeyDown;
};

#endif // !_INPUT_H_
