#ifndef _INPUT_H_
#define _INPUT_H_


#include <windows.h>

#include "Keyboard.h"
#include "GamePad.h"

class Input {

public:
	static std::unique_ptr<DirectX::Keyboard>			pKeyboard;
	static DirectX::Keyboard::State						KEY;
	static DirectX::Keyboard::KeyboardStateTracker		KEY_TRACKER;

	static std::unique_ptr<DirectX::GamePad>			pGamePad;
	static DirectX::GamePad::State						PAD;
	static DirectX::GamePad::ButtonStateTracker			PAD_TRACKER;

	static bool isAnyKeyDown;
};

#endif // !_INPUT_H_
