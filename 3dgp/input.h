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

	static const int									MAX_PLAYER_COUNT = DirectX::GamePad::MAX_PLAYER_COUNT;
	static std::unique_ptr<DirectX::GamePad>			pGamePad;
	static DirectX::GamePad::State						PAD[MAX_PLAYER_COUNT];
	static DirectX::GamePad::ButtonStateTracker			PAD_TRACKER[MAX_PLAYER_COUNT];

	static bool isAnyKeyDown;
};

#endif // !_INPUT_H_
